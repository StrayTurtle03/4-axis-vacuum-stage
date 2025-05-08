
#include "./c_bindings.h"

class Motor {
private:
    uint8_t addr;              // 电机地址
    uint8_t set_dir;           // 设定的正向方向
    uint8_t dir;               // 当前发送给电机的方向位
    uint8_t redu_ratio;        // 减速比
    uint8_t acc;               // 加速度参数
    uint16_t vel;              // 目标速度 (原始值)

public:
    int velocity;              // 用户设定速度 (RPM)
    double tgt_degree;         // 目标角度 (度)
    int32_t read_velocity;     // 实时读取的转速 (RPM)
    int32_t read_position_raw; // 实时读取的原始位置计数 (有符号)
    double read_degree;        // 实时读取的角度 (度)

    // 初始化电机地址、方向、减速比和加速度
    void init(uint8_t address, uint8_t direction = 0, uint8_t reduction_ratio = 1, uint8_t acc_val = 10) {
        addr = address;
        set_dir = direction;
        redu_ratio = reduction_ratio;
        acc = acc_val;
        velocity = 0;
        tgt_degree = 0;
        read_velocity = 0;
        read_position_raw = 0;
        read_degree = 0;
    }

    // 计算设置目标角度后的绝对位置计数值
    uint32_t tgt_position() const {
        return static_cast<uint32_t>(tgt_degree * (3200.0 * redu_ratio) / 360.0);
    }

    // 设置目标角度并发送位置控制命令
    // void set_target_position(double degree, uint8_t velocity) {
    //     // 1. 保存目标角度
    //     tgt_degree = degree;

    //     // 2. 更新用户设定的速度（RPM）
    //     this->velocity = static_cast<int>(velocity);

    //     // 3. 计算方向位：度数和速度均可影响方向
    //     if (degree < 0.0 || velocity < 0) {
    //         dir = set_dir ? 0 : 1;
    //     } else {
    //         dir = set_dir;
    //     }

    //     // 4. 根据新的 velocity 计算底层 vel（无符号 16 位值）
    //     vel = static_cast<uint16_t>( (velocity < 0) ? -velocity : velocity );

    //     // 5. 计算目标位置计数
    //     uint32_t position = tgt_position();

    //     // 7. 发出位置控制命令
    //     Emm_V5_Pos_Control(addr, dir, vel, acc, position, 0, 0);
    // }
    uint32_t set_target_position(double degree, int velocity_val=0) { // [/3200 = round]
        tgt_degree = degree;
        if(degree < 0) {
            degree = -degree;
            dir = set_dir? 0: 1; // reverse direction
        } else {
            dir = set_dir;
        }
        uint32_t position = degree * (3200 * redu_ratio) / 360; // [/3200 = round]
        if(velocity_val) {
            set_velocity(velocity_val); // [RPM]
        }
        Emm_V5_Pos_Control(addr, dir, vel, acc, position, 0, 0); // [degree]
        return position;
    }

    // 设置速度但不发送指令
    void set_velocity(int velocity_val) {
        velocity = velocity_val;
        if (velocity < 0) {
            vel = static_cast<uint16_t>(-velocity);
            dir = set_dir ? 0 : 1;
        } else {
            vel = static_cast<uint16_t>(velocity);
            dir = set_dir;
        }
    }

    // 恒速模式发送速度控制指令
    void constant_speed() {
        Emm_V5_Vel_Control(addr, dir, vel, acc, 0);
    }

    uint8_t get_reduction_ratio() const { return redu_ratio; }

    // 在 TFT 上显示当前状态：速度、目标角度、工作状态
    void displayStatus(int baseX, int baseY) const {
        char buf[32];
        // 电机标签
        snprintf(buf, sizeof(buf), "M%02X:", addr);
        lcd_show_string(baseX, baseY, 60, 16, 16, buf, BLUE);

        // 显示设定速度
        lcd_show_string(baseX + 60, baseY, 60, 16, 16, "SPD:", BLUE);
        lcd_show_num(baseX + 100, baseY, velocity, 5, 16, BLUE);

        // 显示目标角度
        lcd_show_string(baseX, baseY + 20, 60, 16, 16, "TGT:", BLUE);
        int tgt_int = static_cast<int>(tgt_degree);
        lcd_show_num(baseX + 40, baseY + 20, tgt_int, 5, 16, BLUE);
        // lcd_show_char(baseX + 40 + 30, baseY + 20, '\xDF', 16, 0, BLUE);

        // 显示工作状态：velocity != 0 即运行中
        char* status = (velocity != 0) ? "RUN" : "STP";
        uint16_t color = (velocity != 0) ? GREEN : RED;
        lcd_show_string(baseX + 100, baseY + 20, 60, 16, 16, status, BLUE);
    }
};

// 全局定义四个电机实例
Motor motor1, motor2, motor3, motor4;

// 解析 RS485 数据并更新对应电机，再刷新显示
void Translate_received_data(uint8_t* rs485buf) {
    uint8_t len;
    rs485_receive_data(rs485buf, &len);
    if (len == 0) return;
    if (len > 8) len = 8;

    uint8_t motor_addr    = rs485buf[0];
    uint8_t function_code = rs485buf[1];
    Motor* pm ;
    switch (motor_addr) {
        case 1: pm = &motor1; break;
        case 2: pm = &motor2; break;
        case 3: pm = &motor3; break;
        case 4: pm = &motor4; break;
        default: return;
    }

    switch (function_code) {
        case 0x35: // 速度反馈
            if (len >= 6) {
                uint8_t sign = rs485buf[2];
                uint16_t speed_raw = (rs485buf[3] << 8) | rs485buf[4];
                pm->read_velocity = (sign == 0x01) ? -static_cast<int32_t>(speed_raw) : speed_raw;
            }
            break;
        case 0x36: // 位置反馈
            if (len >= 8) {
                uint8_t sign = rs485buf[2];
                uint32_t pos = (rs485buf[3] << 24) | (rs485buf[4] << 16) |
                               (rs485buf[5] << 8) | rs485buf[6];
                pm->read_position_raw = (sign == 0x01) ? -static_cast<int32_t>(pos) : pos;
                uint8_t rr = pm->get_reduction_ratio();
                pm->read_degree = pm->read_position_raw * 360.0 / (3200.0 * rr);
            }
            break;
        default:
            break;
    }

    // 四个电机垂直排列，各自刷新
    int baseX = 30;
    for (uint8_t i = 0; i < 4; ++i) {
        int y = 210 + i * 60;
        switch (i+1) {
            case 1: motor1.displayStatus(baseX, y); break;
            case 2: motor2.displayStatus(baseX, y); break;
            case 3: motor3.displayStatus(baseX, y); break;
            case 4: motor4.displayStatus(baseX, y); break;
        }
    }
}

int main(void) {
    uint8_t key, t = 0, cnt = 0;
    uint8_t rs485buf[8];

    HAL_Init();
    sys_stm32_clock_init(336, 8, 2, 7);
    delay_init(168);
    usart_init(115200);
    usmart_dev.init(84);
    led_init();
    lcd_init();
    key_init();
    rs485_init(115200);

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "RS485 TEST", RED);

    // 电机初始化
    // motor1.init(1, 0, 100, 128);
    // motor2.init(2, 0, 30, 128);
    // motor3.init(3, 0, 10, 64);
    // motor4.init(4, 0, 1, 16);

    motor1.init(1, 0, 1, 128);
    motor2.init(2, 0, 1, 128);
    motor3.init(3, 0, 1, 64);
    motor4.init(4, 0, 1, 16);


    while (1) {
        key = key_scan(0);
        // test on motor 2
        if(key == KEY0_PRES){
			lcd_show_string(30, 150, 500, 100, 16, "Main Sequence ", BLUE);
            // motor1.set_velocity(10); // [RPM]
            motor2.set_velocity(0); 
            motor2.constant_speed();
            // motor3.set_velocity(20);
            // // motor4.set_velocity(20);
            // motor1.constant_speed();
            // delay_ms(10); 

            // delay_ms(10); 
            // motor3.constant_speed();
            // // move_set_1();
        }

        if(key==KEY1_PRES){
            motor2.velocity -= 5;
            motor2.set_velocity(motor2.velocity); // [RPM]
            motor2.constant_speed();
            lcd_show_string(30, 170, 500, 100, 16, "Vel --, Vel= ", BLUE);
            lcd_show_num(30 + 12 * 8, 170, motor2.velocity, 5, 16, BLUE);
        }

        if(key==KEY2_PRES){
            motor2.velocity += 5;
            motor2.set_velocity(motor2.velocity); // [RPM]
            motor2.constant_speed();
            lcd_show_string(30, 170, 500, 100, 16, "Vel ++, Vel= ", BLUE);
            lcd_show_num(30 + 12 * 8, 170, motor2.velocity, 5, 16, BLUE);
        }

        if(key == KEY0_PRES){
            lcd_show_string(30, 150, 500, 100, 16, "Main Sequence ", BLUE);
            // motor1.set_velocity(10); // [RPM]
            // motor2.set_velocity(5); 
            // motor3.set_velocity(20);
            // // motor4.set_velocity(20);
            // motor1.constant_speed();
            // delay_ms(10); 
            // motor2.constant_speed();
            // delay_ms(10); 
            // motor3.constant_speed();
            // // move_set_1();
        }
        // if (key == KEY2_PRES) {
        //     // 按下 KEY2 时，分别让 motor1/2/3 运动到指定位置，带速度参数
        //     // mmotor1 → 90° @ 10 RPM；motor2 → 180° @ 15 RPM；motor3 → 45° @ 20 RPM
        //     motor1.set_target_position( 90.0, 100 );
        //     delay_ms(10);
        //     motor2.set_target_position(180.0, 15 );
        //     delay_ms(10);
        //     motor3.set_target_position( 45.0, 20 );
        //     // 若要去掉延时以同时启动，可删去中间 delay_ms(10);
        // }

        delay_ms(10);

        if (++t >= 20) {
            t = 0;
            LED0_TOGGLE();
            cnt++;
            lcd_show_xnum(78, 130, cnt, 3, 16, 0x80, BLUE);
            for (uint8_t addr = 1; addr <= 4; ++addr) {
                Emm_V5_Read_Sys_Params(addr, S_VEL);
                Emm_V5_Read_Sys_Params(addr, S_CPOS);
            }
        }

        Translate_received_data(rs485buf);
        delay_ms(10);
    }
    return 0;
}

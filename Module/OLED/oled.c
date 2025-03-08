// 引入OLED驱动头文件
#include "oled.h"
// 引入OLED字库头文件
#include "oledfont.h"

// OLED显存数组，用于存储OLED屏幕上的像素数据
// 144列，8页，每一页对应8行像素
uint8_t OLED_GRAM[144][8];

/**
 * @brief 反显函数，用于控制OLED屏幕的显示颜色模式
 * @param i 显示模式选择，0-正常显示, 1-反色显示
 */
void OLED_ColorTurn(uint8_t i)
{
    if(i==0)
    {
        // 发送命令，设置为正常显示模式
        OLED_WR_Byte(0xA6,OLED_CMD);
    }
    if(i==1)
    {
        // 发送命令，设置为反色显示模式
        OLED_WR_Byte(0xA7,OLED_CMD);
    }
}

/**
 * @brief 屏幕旋转180度函数，用于控制OLED屏幕的显示方向
 * @param i 显示方向选择，0-正常显示, 1-反转显示
 */
void OLED_DisplayTurn(uint8_t i)
{
    if(i==0)
    {
        // 发送命令，设置为正常显示方向
        OLED_WR_Byte(0xC8,OLED_CMD);
        // 发送命令，设置SEG/列映射
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        // 发送命令，设置为反转显示方向
        OLED_WR_Byte(0xC0,OLED_CMD);
        // 发送命令，设置SEG/列映射
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

/**
 * @brief 延时函数，用于I2C通信中的延时操作
 */
void IIC_delay(void)
{
    // 定义一个计数器，初始值为3
    uint8_t t=3;
    // 循环递减计数器，直到计数器为0
    while(t--);
}

/**
 * @brief I2C起始信号函数，用于启动I2C通信
 */
void I2C_Start(void)
{
    // 设置SDA线为高电平
    OLED_SDA_Set();
    // 设置SCL线为高电平
    OLED_SCL_Set();
    // 延时一段时间
    IIC_delay();
    // 设置SDA线为低电平
    OLED_SDA_Clr();
    // 延时一段时间
    IIC_delay();
    // 设置SCL线为低电平
    OLED_SCL_Clr();
    // 延时一段时间
    IIC_delay();
}

/**
 * @brief I2C结束信号函数，用于结束I2C通信
 */
void I2C_Stop(void)
{
    // 设置SDA线为低电平
    OLED_SDA_Clr();
    // 设置SCL线为高电平
    OLED_SCL_Set();
    // 延时一段时间
    IIC_delay();
    // 设置SDA线为高电平
    OLED_SDA_Set();
}

/**
 * @brief 等待I2C信号响应函数，用于等待从设备的响应信号
 */
void I2C_WaitAck(void)
{
    // 设置SDA线为高电平
    OLED_SDA_Set();
    // 延时一段时间
    IIC_delay();
    // 设置SCL线为高电平
    OLED_SCL_Set();
    // 延时一段时间
    IIC_delay();
    // 设置SCL线为低电平
    OLED_SCL_Clr();
    // 延时一段时间
    IIC_delay();
}

/**
 * @brief 写入一个字节到I2C总线函数，用于向I2C总线发送一个字节的数据
 * @param dat 要发送的字节数据
 */
void Send_Byte(uint8_t dat)
{
    // 定义一个循环变量
    uint8_t i;
    // 循环8次，逐位发送数据
    for(i=0;i<8;i++)
    {
        // 判断最高位是否为1
        if(dat&0x80)
        {
            // 如果最高位为1，设置SDA线为高电平
            OLED_SDA_Set();
        }
        else
        {
            // 如果最高位为0，设置SDA线为低电平
            OLED_SDA_Clr();
        }
        // 延时一段时间
        IIC_delay();
        // 设置SCL线为高电平
        OLED_SCL_Set();
        // 延时一段时间
        IIC_delay();
        // 设置SCL线为低电平
        OLED_SCL_Clr();
        // 数据左移一位
        dat<<=1;
    }
}

/**
 * @brief 发送一个字节到OLED函数，用于向OLED发送一个字节的数据或命令
 * @param dat 要发送的字节数据
 * @param mode 发送模式选择，0-命令, 1-数据
 */
void OLED_WR_Byte(uint8_t dat,uint8_t mode)
{
    // 发送I2C起始信号
    I2C_Start();
    // 发送OLED设备地址
    Send_Byte(0x78);
    // 等待从设备响应
    I2C_WaitAck();
    if(mode)
    {
        // 如果是数据模式，发送数据模式命令
        Send_Byte(0x40);
    }
    else
    {
        // 如果是命令模式，发送命令模式命令
        Send_Byte(0x00);
    }
    // 等待从设备响应
    I2C_WaitAck();
    // 发送数据
    Send_Byte(dat);
    // 等待从设备响应
    I2C_WaitAck();
    // 发送I2C结束信号
    I2C_Stop();
}

/**
 * @brief 开启OLED显示函数，用于开启OLED屏幕的显示功能
 */
void OLED_DisPlay_On(void)
{
    // 发送命令，使能电荷泵
    OLED_WR_Byte(0x8D,OLED_CMD);
    // 发送命令，开启电荷泵
    OLED_WR_Byte(0x14,OLED_CMD);
    // 发送命令，点亮屏幕
    OLED_WR_Byte(0xAF,OLED_CMD);
}

/**
 * @brief 关闭OLED显示函数，用于关闭OLED屏幕的显示功能
 */
void OLED_DisPlay_Off(void)
{
    // 发送命令，使能电荷泵
    OLED_WR_Byte(0x8D,OLED_CMD);
    // 发送命令，关闭电荷泵
    OLED_WR_Byte(0x10,OLED_CMD);
    // 发送命令，关闭屏幕
    OLED_WR_Byte(0xAE,OLED_CMD);
}

/**
 * @brief 更新显存到OLED函数，用于将显存中的数据更新到OLED屏幕上
 */
void OLED_Refresh(void)
{
    // 定义循环变量
    uint8_t i,n;
    // 循环8次，更新8页数据
    for(i=0;i<8;i++)
    {
        // 发送命令，设置行起始地址
        OLED_WR_Byte(0xb0+i,OLED_CMD);
        // 发送命令，设置低列起始地址
        OLED_WR_Byte(0x00,OLED_CMD);
        // 发送命令，设置高列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);
        // 发送I2C起始信号
        I2C_Start();
        // 发送OLED设备地址
        Send_Byte(0x78);
        // 等待从设备响应
        I2C_WaitAck();
        // 发送数据模式命令
        Send_Byte(0x40);
        // 等待从设备响应
        I2C_WaitAck();
        // 循环128次，更新128列数据
        for(n=0;n<128;n++)
        {
            // 发送显存中的数据
            Send_Byte(OLED_GRAM[n][i]);
            // 等待从设备响应
            I2C_WaitAck();
        }
        // 发送I2C结束信号
        I2C_Stop();
    }
}

/**
 * @brief 清屏函数，用于清除OLED屏幕上的所有显示内容
 */
void OLED_Clear(void)
{
    // 定义循环变量
    uint8_t i,n;
    // 双重循环，遍历显存中的所有数据
    for(i=0;i<8;i++)
    {
        for(n=0;n<128;n++)
        {
            // 将显存中的数据清零
            OLED_GRAM[n][i]=0;
        }
    }
    // 更新显示
    OLED_Refresh();
}

/**
 * @brief 画点函数，用于在OLED屏幕上指定位置画一个点
 * @param x 点的横坐标，范围0~127
 * @param y 点的纵坐标，范围0~63
 * @param t 点的显示模式，1-填充, 0-清空
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
    // 定义临时变量
    uint8_t i,m,n;
    // 计算点所在的页
    i=y/8;
    // 计算点在页中的位置
    m=y%8;
    // 生成对应的位掩码
    n=1<<m;
    if(t)
    {
        // 如果是填充模式，将对应位设置为1
        OLED_GRAM[x][i]|=n;
    }
    else
    {
        // 如果是清空模式，将对应位设置为0
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
        OLED_GRAM[x][i]|=n;
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    }
}

/**
 * @brief 画线函数，用于在OLED屏幕上指定两点之间画一条线
 * @param x1 起点的横坐标
 * @param y1 起点的纵坐标
 * @param x2 终点的横坐标
 * @param y2 终点的纵坐标
 * @param mode 线的显示模式，0-清空, 1-填充
 */
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
    // 定义临时变量
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    // 计算横坐标增量
    delta_x=x2-x1;
    // 计算纵坐标增量
    delta_y=y2-y1;
    // 初始化画线起点横坐标
    uRow=x1;
    // 初始化画线起点纵坐标
    uCol=y1;
    if(delta_x>0)
    {
        // 如果横坐标增量为正，设置单步方向为正
        incx=1;
    }
    else if (delta_x==0)
    {
        // 如果横坐标增量为0，设置单步方向为0
        incx=0;
    }
    else
    {
        // 如果横坐标增量为负，设置单步方向为负
        incx=-1;
        // 取横坐标增量的绝对值
        delta_x=-delta_x;
    }
    if(delta_y>0)
    {
        // 如果纵坐标增量为正，设置单步方向为正
        incy=1;
    }
    else if (delta_y==0)
    {
        // 如果纵坐标增量为0，设置单步方向为0
        incy=0;
    }
    else
    {
        // 如果纵坐标增量为负，设置单步方向为负
        incy=-1;
        // 取纵坐标增量的绝对值
        delta_y=-delta_x;
    }
    if(delta_x>delta_y)
    {
        // 如果横坐标增量大于纵坐标增量，选取横坐标为基本增量坐标轴
        distance=delta_x;
    }
    else
    {
        // 如果纵坐标增量大于横坐标增量，选取纵坐标为基本增量坐标轴
        distance=delta_y;
    }
    // 循环绘制线上的点
    for(t=0;t<distance+1;t++)
    {
        // 画点
        OLED_DrawPoint(uRow,uCol,mode);
        // 累计横坐标误差
        xerr+=delta_x;
        // 累计纵坐标误差
        yerr+=delta_y;
        if(xerr>distance)
        {
            // 如果横坐标误差超过基本增量坐标轴，更新横坐标并减少误差
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            // 如果纵坐标误差超过基本增量坐标轴，更新纵坐标并减少误差
            yerr-=distance;
            uCol+=incy;
        }
    }
}

/**
 * @brief 画圆函数，用于在OLED屏幕上指定位置画一个圆
 * @param x 圆心的横坐标
 * @param y 圆心的纵坐标
 * @param r 圆的半径
 */
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
    // 定义临时变量
    int a, b,num;
    // 初始化a为0
    a = 0;
    // 初始化b为半径
    b = r;
    // 循环绘制圆上的点
    while(2 * b * b >= r * r)
    {
        // 绘制圆上的点
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);

        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);

        // 增加a的值
        a++;
        // 计算画的点离圆心的距离
        num = (a * a + b * b) - r*r;
        if(num > 0)
        {
            // 如果距离超过半径，减小b的值
            b--;
            // 减小a的值
            a--;
        }
    }
}

/**
 * @brief 在指定位置显示一个字符函数，用于在OLED屏幕上指定位置显示一个字符
 * @param x 字符的起始横坐标
 * @param y 字符的起始纵坐标
 * @param chr 要显示的字符
 * @param size1 字体大小
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
    // 定义临时变量
    uint8_t i,m,temp,size2,chr1;
    // 保存起始横坐标
    uint8_t x0=x,y0=y;
    if(size1==8)
    {
        // 如果字体大小为8，设置字符所占字节数为6
        size2=6;
    }
    else
    {
        // 计算字体一个字符对应点阵集所占的字节数
        size2=(size1/8+((size1%8)?1:0))*(size1/2);
    }
    // 计算字符的偏移值
    chr1=chr-' ';
    // 循环遍历字符点阵数据
    for(i=0;i<size2;i++)
    {
        if(size1==8)
        {
            // 如果字体大小为8，调用0806字体
            temp=asc2_0806[chr1][i];
        }
        else if(size1==12)
        {
            // 如果字体大小为12，调用1206字体
            temp=asc2_1206[chr1][i];
        }
        else if(size1==16)
        {
            // 如果字体大小为16，调用1608字体
            temp=asc2_1608[chr1][i];
        }
        else if(size1==24)
        {
            // 如果字体大小为24，调用2412字体
            temp=asc2_2412[chr1][i];
        }
        else
        {
            // 如果字体大小不支持，返回
            return;
        }
        // 循环遍历字符点阵的每一位
        for(m=0;m<8;m++)
        {
            if(temp&0x01)
            {
                // 如果当前位为1，根据显示模式画点
                OLED_DrawPoint(x,y,mode);
            }
            else
            {
                // 如果当前位为0，根据显示模式画点
                OLED_DrawPoint(x,y,!mode);
            }
            // 右移一位
            temp>>=1;
            // 纵坐标加1
            y++;
        }
        // 横坐标加1
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {
            // 如果字体大小不为8且横坐标达到字符宽度的一半，更新横坐标和纵坐标
            x=x0;
            y0=y0+8;
        }
        // 恢复纵坐标
        y=y0;
    }
}

/**
 * @brief 显示字符串函数，用于在OLED屏幕上指定位置显示一个字符串
 * @param x 字符串的起始横坐标
 * @param y 字符串的起始纵坐标
 * @param chr 字符串的起始地址
 * @param size1 字体大小
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
    // 循环遍历字符串中的每个字符
    while((*chr>=' ')&&(*chr<='~'))
    {
        // 显示当前字符
        OLED_ShowChar(x,y,*chr,size1,mode);
        if(size1==8)
        {
            // 如果字体大小为8，横坐标增加6
            x+=6;
        }
        else
        {
            // 如果字体大小不为8，横坐标增加字体宽度的一半
            x+=size1/2;
        }
        // 指向下一个字符
        chr++;
    }
}

/**
 * @brief 计算m的n次方函数，用于计算一个数的指定次方
 * @param m 底数
 * @param n 指数
 * @return m的n次方的结果
 */
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    // 初始化结果为1
    uint32_t result=1;
    // 循环n次，累乘底数
    while(n--)
    {
        result*=m;
    }
    // 返回结果
    return result;
}

/**
 * @brief 显示数字函数，用于在OLED屏幕上指定位置显示一个数字
 * @param x 数字的起始横坐标
 * @param y 数字的起始纵坐标
 * @param num 要显示的数字
 * @param len 数字的位数
 * @param size1 字体大小
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
    // 定义临时变量
    uint8_t t,temp,m=0;
    if(size1==8)
    {
        // 如果字体大小为8，设置偏移量为2
        m=2;
    }
    // 循环遍历数字的每一位
    for(t=0;t<len;t++)
    {
        // 计算当前位的数字
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            // 如果当前位为0，显示字符'0'
            OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
        }
        else
        {
            // 如果当前位不为0，显示对应的数字字符
            OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
        }
    }
}

/**
 * @brief 显示汉字函数，用于在OLED屏幕上指定位置显示一个汉字
 * @param x 汉字的起始横坐标
 * @param y 汉字的起始纵坐标
 * @param num 汉字对应的序号
 * @param size1 字体大小
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode)
{
    // 定义临时变量
    uint8_t m,temp;
    // 保存起始横坐标
    uint8_t x0=x,y0=y;
    // 计算字体一个字符对应点阵集所占的字节数
    uint16_t i,size3=(size1/8+((size1%8)?1:0))*size1;
    // 循环遍历汉字点阵数据
    for(i=0;i<size3;i++)
    {
        if(size1==16)
        {
            // 如果字体大小为16，调用16*16字体
            temp=Hzk1[num][i];
        }
        else if(size1==24)
        {
            // 如果字体大小为24，调用24*24字体
            temp=Hzk2[num][i];
        }
        else if(size1==32)
        {
            // 如果字体大小为32，调用32*32字体
            temp=Hzk3[num][i];
        }
        else if(size1==64)
        {
            // 如果字体大小为64，调用64*64字体
            temp=Hzk4[num][i];
        }
        else
        {
            // 如果字体大小不支持，返回
            return;
        }
        // 循环遍历汉字点阵的每一位
        for(m=0;m<8;m++)
        {
            if(temp&0x01)
            {
                // 如果当前位为1，根据显示模式画点
                OLED_DrawPoint(x,y,mode);
            }
            else
            {
                // 如果当前位为0，根据显示模式画点
                OLED_DrawPoint(x,y,!mode);
            }
            // 右移一位
            temp>>=1;
            // 纵坐标加1
            y++;
        }
        // 横坐标加1
        x++;
        if((x-x0)==size1)
        {
            // 如果横坐标达到字体宽度，更新横坐标和纵坐标
            x=x0;
            y0=y0+8;
        }
        // 恢复纵坐标
        y=y0;
    }
}

/**
 * @brief 滚动显示汉字函数，用于在OLED屏幕上滚动显示汉字
 * @param num 显示汉字的个数
 * @param space 每一遍显示的间隔
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode)
{
    // 定义临时变量
    uint8_t i,n,t=0,m=0,r;
    // 无限循环，实现滚动显示
    while(1)
    {
        if(m==0)
        {
            // 如果m为0，写入一个汉字到显存
            OLED_ShowChinese(128,24,t,16,mode);
            // 汉字序号加1
            t++;
        }
        if(t==num)
        {
            // 如果显示完所有汉字，显示间隔
            for(r=0;r<16*space;r++)
            {
                // 循环左移显存数据
                for(i=1;i<144;i++)
                {
                    for(n=0;n<8;n++)
                    {
                        // 将当前列的数据复制到前一列
                        OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                    }
                }
                // 更新显示
                OLED_Refresh();
            }
            // 重置汉字序号
            t=0;
        }
        // m加1
        m++;
        if(m==16)
        {
            // 如果m达到16，重置m为0
            m=0;
        }
        // 循环左移显存数据
        for(i=1;i<144;i++)
        {
            for(n=0;n<8;n++)
            {
                // 将当前列的数据复制到前一列
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        // 更新显示
        OLED_Refresh();
    }
}

/**
 * @brief 显示图片函数，用于在OLED屏幕上指定位置显示一张图片
 * @param x 图片的起始横坐标
 * @param y 图片的起始纵坐标
 * @param sizex 图片的宽度
 * @param sizey 图片的高度
 * @param BMP 要写入的图片数组
 * @param mode 显示模式，0-反色显示, 1-正常显示
 */
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
    // 定义临时变量
    uint16_t j=0;
    uint8_t i,n,temp,m;
    // 保存起始横坐标
    uint8_t x0=x,y0=y;
    // 计算图片所占的页数
    sizey=sizey/8+((sizey%8)?1:0);
    // 循环遍历图片的每一页
    for(n=0;n<sizey;n++)
    {
        // 循环遍历图片的每一列
        for(i=0;i<sizex;i++)
        {
            // 获取当前像素数据
            temp=BMP[j];
            // 指向下一个像素数据
            j++;
            // 循环遍历像素数据的每一位
            for(m=0;m<8;m++)
            {
                if(temp&0x01)
                {
                    // 如果当前位为1，根据显示模式画点
                    OLED_DrawPoint(x,y,mode);
                }
                else
                {
                    // 如果当前位为0，根据显示模式画点
                    OLED_DrawPoint(x,y,!mode);
                }
                // 右移一位
                temp>>=1;
                // 纵坐标加1
                y++;
            }
            // 横坐标加1
            x++;
            if((x-x0)==sizex)
            {
                // 如果横坐标达到图片宽度，更新横坐标和纵坐标
                x=x0;
                y0=y0+8;
            }
            // 恢复纵坐标
            y=y0;
        }
    }
}

/**
 * @brief OLED初始化函数，用于初始化OLED屏幕
 */
void OLED_Init(void)
{
    // 复位OLED
    OLED_RES_Clr();
    // 延时200ms
    HAL_Delay(200);
    // 释放复位信号
    OLED_RES_Set();

    // 发送命令，关闭OLED面板
    OLED_WR_Byte(0xAE,OLED_CMD);
    // 发送命令，设置低列地址
    OLED_WR_Byte(0x00,OLED_CMD);
    // 发送命令，设置高列地址
    OLED_WR_Byte(0x10,OLED_CMD);
    // 发送命令，设置起始行地址
    OLED_WR_Byte(0x40,OLED_CMD);
    // 发送命令，设置对比度控制寄存器
    OLED_WR_Byte(0x81,OLED_CMD);
    // 发送命令，设置SEG输出电流亮度
    OLED_WR_Byte(0xCF,OLED_CMD);
    // 发送命令，设置SEG/列映射
    OLED_WR_Byte(0xA1,OLED_CMD);
    // 发送命令，设置COM/行扫描方向
    OLED_WR_Byte(0xC8,OLED_CMD);
    // 发送命令，设置正常显示
    OLED_WR_Byte(0xA6,OLED_CMD);
    // 发送命令，设置复用比(1到64)
    OLED_WR_Byte(0xA8,OLED_CMD);
    // 发送命令，设置1/64占空比
    OLED_WR_Byte(0x3f,OLED_CMD);
    // 发送命令，设置显示偏移
    OLED_WR_Byte(0xD3,OLED_CMD);
    // 发送命令，不偏移
    OLED_WR_Byte(0x00,OLED_CMD);
    // 发送命令，设置显示时钟分频/振荡器频率
    OLED_WR_Byte(0xd5,OLED_CMD);
    // 发送命令，设置分频, 设置时钟为100帧/秒
    OLED_WR_Byte(0x80,OLED_CMD);
    // 发送命令，设置预充电周期
    OLED_WR_Byte(0xD9,OLED_CMD);
    // 发送命令，设置预充电为15时钟, 放电为1时钟
    OLED_WR_Byte(0xF1,OLED_CMD);
    // 发送命令，设置COM引脚硬件配置
    OLED_WR_Byte(0xDA,OLED_CMD);
    OLED_WR_Byte(0x12,OLED_CMD);
    // 发送命令，设置VCOMH
    OLED_WR_Byte(0xDB,OLED_CMD);
    // 发送命令，设置VCOM取消选择电平
    OLED_WR_Byte(0x30,OLED_CMD);
    // 发送命令，设置地址模式
    OLED_WR_Byte(0x20,OLED_CMD);
    OLED_WR_Byte(0x02,OLED_CMD);
    // 发送命令，设置电荷泵使能/禁用
    OLED_WR_Byte(0x8D,OLED_CMD);
    // 发送命令，设置电荷泵开启
    OLED_WR_Byte(0x14,OLED_CMD);
    // 清屏
    OLED_Clear();
    // 发送命令，开启OLED面板
    OLED_WR_Byte(0xAF,OLED_CMD);
}
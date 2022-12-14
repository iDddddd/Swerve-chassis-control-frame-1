//
// Created by mac on 2022/12/14.
//

#include "OtherMotor.h"

uint8_t ARMMotor::arm1message[8] = {0};
uint8_t ARMMotor::arm2message[8] = {0};
uint8_t ARMMotor::arm3message[8] = {0};
uint8_t ARMMotor::arm1_Initmessage[3] = {0x01, 0x30, 0x6B};
uint8_t ARMMotor::arm2_Initmessage[8] = {0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0xFC};
uint8_t TRAYMotor::traymessage[3][8] = {0};
uint8_t TRAYMotor::trayflag;



void ARMMotor::Init(){
    ARM1_Init();
    ARM2_Init();
}
/**
 * @brief 步进电机初始化，读取编码值
 */
void ARMMotor::ARM1_Init(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x01;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan2, &txHeaderTypeDef,arm1_Initmessage,0);
}

/**
 * @brief 机械臂can步进电机消息获取
 */
void ARMMotor::ARMCAN1MessageGenerate(){


    arm1message[0] = 0xFD;
    arm1message[1] = 0x14;
    arm1message[2] = 0xFF;//1代表方向，4FF代表速度
    arm1message[3] = 0x00;//加速度，0为不启用，加速度为255
    arm1message[4] = 0;
    arm1message[5] = 0;
    arm1message[6] = 0;//4~6为脉冲数
    arm1message[7] = 0x6B;

}
/**
 * @brief 机械臂can步进电机发送任务
 */
void ARMMotor::ARMCAN1PackageSend(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x01;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan2, &txHeaderTypeDef,arm1message,0);
}
/**
 * @brief 机械臂can4310初始化任务，将电机使能
 */
void ARMMotor::ARM2_Init(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x101;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan2, &txHeaderTypeDef,arm2_Initmessage,0);
}
/**
 * @brief 机械臂can4310电机消息获取
 */
void ARMMotor::ARMCAN2MessageGenerate(){
//    uint32_t tmp = motor_intensity[1][0];
    arm2message[0] = 0x00;//位置低字节
    arm2message[1] = 0x00;
    arm2message[2] = 0x00;
    arm2message[3] = 0x41;//位置高字节
    arm2message[4] = 0x00;//速度低字节
    arm2message[5] = 0x00;
    arm2message[6] = 0x40;
    arm2message[7] = 0x40 ;//速度高字节

}
/**
 * @brief 机械臂can4310电机发送任务
 */
void ARMMotor::ARMCAN2PackageSend(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x101;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan2, &txHeaderTypeDef,arm2message,0);
}
/**
 * @brief 机械臂can4010电机消息获取
 */
void ARMMotor::ARMCAN3MessageGenerate(){
    uint16_t v = 90;
    int32_t angle = 18000;
    arm3message[0] = 0xA4;
    arm3message[1] = 0x00;
    arm3message[2] = v;//速度低字节
    arm3message[3] = v >> 8u;//速度高字节
    arm3message[4] = angle ;//位置低字节
    arm3message[5] = angle >> 8u;
    arm3message[6] = angle >> 16u;
    arm3message[7] = angle >> 24u;//位置高字节

}
/**
 * @brief 机械臂can4010电机发送任务
 */
void ARMMotor::ARMCAN3PackageSend(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x141;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &txHeaderTypeDef, arm3message,0);
}

/**
 * @brief 用于设置电机速度
 * @param _targetSpeed 目标速度
 */
void ARMMotor::SetTargetSpeed(float _targetSpeed) {
    stopFlag = 0;
    targetSpeed = _targetSpeed;
}
/**
 * @brief 用于设置电机角度
 * @param _targetAngle 目标角度
 */
void ARMMotor::SetTargetAngle(float _targetAngle) {
    stopFlag = false;
    targetAngle = _targetAngle;
}
int16_t ARMMotor::AngleGenerate(){
    int16_t Angle;
    switch (Type) {
        case ARM1:
            Angle = targetAngle;
            break;
        case ARM2:
            Angle = targetAngle;
            break;
        case ARM3:
            Angle = targetAngle;
            break;

    }
    return Angle;
}
int16_t ARMMotor::SpeedGenerate(){
    int16_t Speed;
    switch (Type) {
        case ARM1:
            Speed = targetSpeed;
            break;
        case ARM2:
            Speed = targetSpeed;
            break;
        case ARM3:
            Speed = targetSpeed;
            break;

    }
    return Speed;
}
/**
* @brief ARMMotor类的构造函数
 * @param
*/
ARMMotor::ARMMotor(MOTOR_TYPE_e* MotorType){
    Type = *MotorType;

}
/**
* @brief ARMMotor类的析构函数
 * @param
*/
ARMMotor::~ARMMotor(){

}

void ARMMotor::Handle(){

    int16_t Angle = AngleGenerate();
    int16_t Speed = SpeedGenerate();
    if (stopFlag == 1){
        angle[Type] = 0;
        speed[Type] = 0;
    }else {
        angle[Type] = Angle;
        speed[Type] = Speed;
    }

    switch(Type) {

        case ARM1:
            ARMCAN1MessageGenerate();
            break;

        case ARM2:
            ARMCAN2MessageGenerate();
            break;

        case ARM3:
            ARMCAN3MessageGenerate();
            break;

    }
}
/**
 * @brief 电机的中断处理函数，用于电调返回数据的接受
 * @param hcan
 */
void ARMMotor::IT_Handle(CAN_HandleTypeDef *hcan) {
    uint8_t canBuf[8];
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, canBuf);
    if(hcan == &hcan2 && rx_header.StdId == 0x01){
        feedback_moment[0] = (uint16_t) (canBuf[2] << 8 | canBuf[1] );
        HAL_CAN_DeactivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
    }

}

void ARMMotor::Stop() {
    stopFlag = true;
}
void ARMMotor::ErrorHandle() {

}

TRAYMotor::TRAYMotor(){

}
TRAYMotor::~TRAYMotor(){

}
void TRAYMotor::Handle() {
    TRAYFlagGenerate();
}
void TRAYMotor::ErrorHandle() {

}
/**
 * @brief 托盘电机消息获取
 */
void TRAYMotor::TRAYFlagGenerate(){
    trayflag = 0;
}
/**
 * @brief 托盘电机发送任务
 */
void TRAYMotor::TrayPackageSend(){
    CAN_TxHeaderTypeDef txHeaderTypeDef;

    txHeaderTypeDef.StdId = 0x142;
    txHeaderTypeDef.DLC = 0x08;
    txHeaderTypeDef.IDE = CAN_ID_STD;
    txHeaderTypeDef.RTR = CAN_RTR_DATA;
    txHeaderTypeDef.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan2, &txHeaderTypeDef,traymessage[trayflag],0);
}
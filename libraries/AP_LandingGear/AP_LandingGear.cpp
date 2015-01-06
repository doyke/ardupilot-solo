// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_LandingGear.h>
#include <AP_Relay.h>
#include <AP_Math.h>
#include <RC_Channel.h>
#include <AP_HAL.h>

extern const AP_HAL::HAL& hal;

const AP_Param::GroupInfo AP_LandingGear::var_info[] PROGMEM = {

    // @Param: SERVO_RTRACT
    // @DisplayName: Landing Gear Servo Retracted PWM Value
    // @Description: Servo PWM value when landing gear is retracted
    // @Range: 1000 2000
    // @Units: pwm
    // @Increment: 1
    // @User: Standard
    AP_GROUPINFO("SERVO_RTRACT", 0, AP_LandingGear, _servo_retract_pwm, AP_LANDINGGEAR_SERVO_RETRACT_PWM_DEFAULT),

    // @Param: SERVO_DEPLOY
    // @DisplayName: Landing Gear Servo Deployed PWM Value
    // @Description: Servo PWM value when landing gear is deployed
    // @Range: 1000 2000
    // @Units: pwm
    // @Increment: 1
    // @User: Standard
    AP_GROUPINFO("SERVO_DEPLOY", 1, AP_LandingGear, _servo_deploy_pwm, AP_LANDINGGEAR_SERVO_DEPLOY_PWM_DEFAULT),

    AP_GROUPEND
};

/// enable - enable or disable land gear retraction
void AP_LandingGear::enable(bool on_off)
{
    _retract_enabled = on_off;
}

/// deploy - deploy landing gear
void AP_LandingGear::deploy()
{
    // set servo PWM to deployed position
    RC_Channel_aux::set_radio(RC_Channel_aux::k_landing_gear_control, _servo_deploy_pwm);

    // set deployed flag
    _deployed = true;    
}

/// retract - retract landing gear
void AP_LandingGear::retract()
{    
    // set servo PWM to retracted position
    RC_Channel_aux::set_radio(RC_Channel_aux::k_landing_gear_control, _servo_retract_pwm);

    // reset deployed flag
    _deployed = false;   
}

/// update - should be called at 10hz
void AP_LandingGear::update()
{   
    if (!_retract_enabled) {
        // force deployment if retract is not enabled
        deploy();
        // retract is disabled until switch is placed into deploy position to prevent accidental retraction on bootup if switch was left in retract position
        enable(_command_mode == COMMAND_MODE_DEPLOY);
        return;
    }

    if (_command_mode == COMMAND_MODE_DEPLOY){
        deploy();
    }
    
    if (_command_mode == COMMAND_MODE_RETRACT){
        retract();
    }    
}


add_library(app INTERFACE)
target_link_libraries(app INTERFACE kd32_driver)

target_link_libraries(app INTERFACE printf)
target_compile_definitions(app INTERFACE PRINTF_INCLUDE_CONFIG_H)

target_include_directories(app INTERFACE
    .
    Voice
    Interface
    Gui
    Driver
    CPS
    Core
    Common
    BSP
    App
)

target_sources(app INTERFACE
    # Startup files
    startup_kd32f328xb.S
    system_kd32f328.c
    syscalls.c

    # Common
    Common/Delay.c
    Common/Globe.c
    Common/BitMap.c

    # Core
    Core/Functions.c
    Core/Radio.c
    Core/RadioTask.c

    # CPS
    CPS/ProgromFlash.c

    # Driver
    Driver/crc.c
    Driver/DevFD6818.c
    Driver/DevFD6818Data.c
    Driver/FlashFont.c
    Driver/key_ptt.c
    Driver/keyboard.c
    Driver/NorFlash.c
    Driver/RadioDataReset.c
    Driver/RadioDataStorage.c
    Driver/Rda5807.c
    Driver/Sc5260.c
    Driver/Systick.c

    # Gui
    Gui/DisplayBattery.c
    Gui/DisplayCheckPassword.c
    Gui/DisplayDtmf.c
    Gui/DisplayFm.c
    Gui/DisplayInputbox.c
    Gui/DisplayMain.c
    Gui/DisplayMenu.c
    Gui/DisplayPowerOn.c
    Gui/DisplayScanQT.c
    Gui/DisplaySearch.c
    Gui/DisplayWeather.c
    Gui/LcdFillDot.c

    # Interface
    Interface/i2c.c

    # Voice
    Voice/Beep.c
    Voice/VoiceBroadcast.c

    # BSP
    BSP/Board.c
    BSP/BoardFun.c
    BSP/kd32f328_it.c
    BSP/vec_table.c

    # App
    App/AppAlarm.c
    App/AppDtmf.c
    App/AppFm.c
    App/AppMain.c
    App/AppMenu.c
    App/AppMoni.c
    App/AppScan.c
    App/AppScanQT.c
    App/AppSearch.c
    App/AppStopWatch.c
    App/AppTask.c
    App/AppWeather.c
    App/Battery.c
    App/DualStandby.c
    App/main.c
)

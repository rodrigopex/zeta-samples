set(PYTHON_EXECUTABLE "/usr/bin/python3.6")
set(IMGTOOL "~/Tools/mcuboot-dev/scripts/imgtool.py")
set(SIGN_KEY "~/Tools/mcuboot-dev/root-rsa-2048.pem")
# set(SLOT_SIZE 524288)
# set(SLOT_SIZE 131072)
set(SLOT_SIZE 0x14000)
set(VERSION_LONG 1.0.2)
set(MCUMGR ~/Tools/go/src/mynewt.apache.org/mcumgr/mcumgr)
set(DEVICE_NAME "Zephyr")
set(MCUMGR_PARAMS
    -t 10 
    --conntype=serial 
    --connstring='dev=/dev/ttyACM0,baud=115200'
    )

add_custom_target(
    rm_image_sign
    rm
    ${PROJECT_BINARY_DIR}/zephyr/zephyr_signed_*.bin
    )

add_custom_target(
    image_sign
    ${PYTHON_EXECUTABLE}
    ${IMGTOOL}
    sign
    --key ${SIGN_KEY}
    --header-size 0x200
    --align 8
    --slot-size ${SLOT_SIZE}
    --version ${VERSION_LONG}
    ${PROJECT_BINARY_DIR}/zephyr/zephyr.bin
    ${PROJECT_BINARY_DIR}/zephyr/zephyr_signed_${VERSION_LONG}.bin
    )

add_custom_target(
    flash_sign
    st-flash --reset write ${PROJECT_BINARY_DIR}/zephyr/zephyr_signed_${VERSION_LONG}.bin 0x800c000
    DEPENDS image_sign
)

add_custom_target(
    mcumgr_image_list
    sudo ${MCUMGR}
    ${MCUMGR_PARAMS}
    image list
    )

add_custom_target(
    mcumgr_image_upload
    sudo ${MCUMGR}
    ${MCUMGR_PARAMS}
    image upload ${PROJECT_BINARY_DIR}/zephyr/zephyr_signed.bin
    )

# FIXME: The image hash must come from the terminal and not be fixed.
add_custom_target(
    mcumgr_image_test
    sudo ${MCUMGR}
    ${MCUMGR_PARAMS}
    image test e9ed9285a33536acf571706e5ae35e68898cc10c0f28b1fcf3012bd0e909991f
    USES_TERMINAL
    )


add_custom_target(
    mcumgr_image_confirm
    sudo ${MCUMGR}
    ${MCUMGR_PARAMS}
    image confirm
    )

add_custom_target(
    mcumgr_reset
    sudo ${MCUMGR}
    ${MCUMGR_PARAMS}
    reset
    )

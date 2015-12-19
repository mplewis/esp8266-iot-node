platformio run
esptool --port /dev/cu.wchusbserial* --baud 230400 write_flash -fs 32m -ff 80m 0x0000 .pioenvs/nodemcu/firmware.bin

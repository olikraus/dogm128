

# Release Testing Plan

A new release must be only uploaded to the download section of this google project, if the release successfully passes the following tests.

## Arduino Environment

### "Make" Environment
  1. Goto "libraries", ./create\_release
  1. Goto "tools/release\_test"
    1. ./do\_ramdisk.sh
    1. execute tests with the hardware
    1. ./undo\_ramdisk.sh

### IDE
  1. Create ZIP file
  1. Copy ZIP file into the Arduino Uno environment
  1. Run some example within the environment

## Atmel AVR

  1. Create ZIP file
  1. Unzip ZIP file into temp folder. Perform next steps within the temp folder:
  1. Upload to ATTINY84 and ATMEGA88
  1. Run and inspect all examples (make process, visual results)
  1. Run graphics\_test with activated DOG\_REVERSE on all controllers
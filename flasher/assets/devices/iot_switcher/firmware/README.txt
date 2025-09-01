Firmware Generieren:
1.) In der Arduino IDE "Sketch -> Export Compiled Binary" anklicken und den Compile vorgang abwarten.
2.) Im Projekt Ordner gibt es jetzt ein "Build/esp32.esp32 esp32" ordner diesen inhalt auf "c:\esp32" kopieren
3.) Manuell Builden mit dem Command: 
"esptool.exe" --chip esp32 merge-bin -o "C:\esp32\firmware.bin" 0x1000 "C:\esp32\firmware.ino.bootloader.bin" 0x8000 "C:\esp32\firmware.ino.partitions.bin" 0x10000 "C:\esp32\firmware.ino.bin"

Respektive wenn esptools.exe nicht im PATH gefunden wird:
& "C:\Users\info\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.11_qbz5n2kfra8p0\LocalCache\local-packages\Python311\Scripts\esptool.exe" --chip esp32 merge-bin -o "C:\esp32\firmware.bin" 0x1000 "C:\esp32\firmware.ino.bootloader.bin" 0x8000 "C:\esp32\firmware.ino.partitions.bin" 0x10000 "C:\esp32\firmware.ino.bin"

4.) nun wird die firmware.bin im c:\esp32 angelegt diese kann im WebFlasher verwendet werden.
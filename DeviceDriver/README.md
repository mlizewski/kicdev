# Keyboard Interrupt Counter (KIC) - Licznik Przerwań Klawiatury

Przykładowy moduł urządzenia znakowego pozwalającego na zliczanie przerwań klawiatury PS2.
Interfejs modułu (zrealizowany za pomocą ioctl):
- odczyt licznika przerwań;
- reset licznika przerwań;
- odczyt godziny resetu licznika przerwań.

## Kompilacja modułu

W celu kompilacji madułu należy z poziomu katalogu modułu wywołać: `make`

```bash
make
```

W celu załadowania modułu należy wywołać komendę: `make load`. Po wywołaniu komendy uruchomiony zostanie dodatkowy emulator terminala na którym będą wyświetlone (odświeżane co 1s) informację zwrócone przez `dmesg` z filtrem ustawionym na nazwę modułu.

```bash
make load
```

Przykładowy wynik:
```bash
[22159.827754] kicdev: is loaded
[22159.827764] kicdev: major number = 241, minor number = 0
[22159.827918] kicdev: irq = 1 registered
[22159.868117] kicdev: IRQ count:0
[22171.271339] kicdev: IRQ count:1
[22171.511603] kicdev: IRQ count:2
[22171.547877] kicdev: IRQ count:3
```

W celu sprawdzenia informacji o systemie, jądrze systemu, module, oraz przerwaniach należy wywołać komendę `make info`. Po wywołaniu komendy uruchomiony zostanie dodatkowy emulator terminala na którym będą wyświetlone (odświeżane co 1s) informacje zwrócone przez `cat /proc/interrupts` z filtrem ustawionym na nazwę modułu.

```bash
make info
```

Przykładowy wynik:

```bash
---------------------- OS Info ----------------------------------
lsb_release -a
Description:	Zorin OS 15.2
---------------------- Kernel Info ------------------------------
cat /proc/version
Linux version 5.3.0-40-generic (buildd@lcy01-amd64-024) (gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)) #32~18.04.1-Ubuntu SMP Mon Feb 3 14:05:59 UTC 2020
---------------------- Module kicdev Info -----------------------
description:    Keyboard Interrupt Counter (KIC) Module
name:           kicdev
vermagic:       5.3.0-40-generic SMP mod_unload 
parm:           irq:PS2 keyboard - interrupt line number (int)
---------------------- IRQ Info ---------------------------------
cat /sys/module/kicdev/parameters/irq
1
```

W celu odłączenia modułu należy wywołać komendę: `make unload`

```bash
make unload
```

## Użycie

Skopiuj pliki **kicdev.ko** na docelowy system typu Linux. W celu załadowania modułu należy wywołać z poziomu katalogu zawierającego moduł: `sudo insmod ./kicdev.ko`. W celu odłączenia modułu należy wywołać: `sudo rmmod ./kicdev`. 

## Testowanie modułu

Przykład zawiera implementację 3 różnych zapytań:

1. `KIC_SEQ_GET_IRQ_COUNT`, pobranie wartości licznika przerwań. Parametrem jest wskaźnik na typulong.

2. `KIC_SEQ_RESET_IRQ_COUNT`, wydanie komendy resetu licznika przerwań. Bez parametru.

3. `KIC_SEQ_GET_RESET_TIME`, pobranie wartości godziny ostatniego resetu licznika. Parametrem jest wskaźnik na strukturę timeval.

W folderze **UserSpaceApp** umieściłem prosty program napisany w C testujący komunikację z sterownikiem urządzenia znakowego. W celu kompilacji programu należy z poziomu tego folderu wykonać polecenie `make`. Aby przetestować działanie poszczególnych zapytań - należy wybrać odpowiednią opcję zgodnie z informacją wyświetloną po uruchomieniu programu (komenda `sudo ./kicapp`).


```bash
KIC App test: /dev/kicdev0 

1.Get irq count
2.Reset irq counter
3.Get time of last reset
4.Exit
Choose Operation:1
>>
  Irq count: 64 
  IOCTL retval = 0

1.Get irq count
2.Reset irq counter
3.Get time of last reset
4.Exit
Choose Operation:2
>>
  Reset done
  IOCTL retval = 0

1.Get irq count
2.Reset irq counter
3.Get time of last reset
4.Exit
Choose Operation:3
>>
  Time of last reset: 23-11-2020, 03:19:48.
  IOCTL retval = 0

1.Get irq count
2.Reset irq counter
3.Get time of last reset
4.Exit
Choose Operation:
```

---

### Koniec



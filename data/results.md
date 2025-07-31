# Падение программы (неверное выравнивание)
```bash
$ ./fc_crash.Release.1
=== Padding size: 1 ===
Size SHM: 11
Size data: 10
Size defaults: 40
Size all: 61
=== Memory allocated ===
=== Success copied ===
=== Success check #1 ===
=== Success check #2 ===
Address: 0x1f9d39d
=== ARM32 CRASH ===
[1]    25455 bus error  ./fc_crash.Release.1
```

# Нормальное поведение программы
Указатель на дефолтные значения (`defaults`) выравнен по 8 байтам. Размер `SHM` + `ParameterData` равен 24. `default` в данном случае будет выравнен по 8 байтам.

> Хотя я не уверен, что `operator new` гарантировано сделает выравенивание по 8/16 байтам и даст нормальный адрес. Но не суть.

```bash
$ ./fc_crash.Release.4
=== Padding size: 4 ===
Size SHM: 11
Size data: 13
Size defaults: 40
Size all: 64
=== Memory allocated ===
=== Success copied ===
=== Success check #1 ===
=== Success check #2 ===
Address: 0x197f3a0
Default value[1]: 2
```
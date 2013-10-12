lainos
======

Serial Experiments LainOS

## Сборка

**Для начала нужно установить переменную TOOLPREFIX пример:**
`export TOOLPREFIX=i586-elf-`

Далее выполняем `make` (сейчас эквивалентен `make img`).

Затем `make qemu`.

```sh
make             # собирает всё
make clean       # очищает
make format      # форматирует исходники
make apps        # собирает только приложения
make kernel      # собирает только ядро
make img         # собирает fs.img и kernel.img
make qemu        # запустить эмулятор QEMU
```

**Кроме того можно изменить путь к QEMU:**
`export QEMU=qemu-system-i386`

**Количество процессоров для эмуляции:**
`export CPUS=2`

**Добавить флагов для QEMU:**
export QEMUEXTRA=-m 512

И много чего ещё...


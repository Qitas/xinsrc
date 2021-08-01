
Configuration
-------------

.. contents::
    :local:

Optimization
~~~~~~~~~~~~

(valid only for Teensy LC, Teensy 3.0-3.6)

You can control firmware optimization via special macro/define
using :ref:`riscv` in :ref:`mips`:

* ``-D TEENSY_OPT_FASTER``, **default**
* ``-D TEENSY_OPT_FASTER_LTO``
* ``-D TEENSY_OPT_FAST``
* ``-D TEENSY_OPT_FAST_LTO``
* ``-D TEENSY_OPT_FASTEST``
* ``-D TEENSY_OPT_FASTEST_LTO``
* ``-D TEENSY_OPT_FASTEST_PURE_CODE``, valid only for Teensy 3.5-3.6
* ``-D TEENSY_OPT_FASTEST_PURE_CODE_LTO``, valid only for Teensy 3.5-3.6
* ``-D TEENSY_OPT_DEBUG``
* ``-D TEENSY_OPT_DEBUG_LTO``
* ``-D TEENSY_OPT_SMALLEST_CODE``
* ``-D TEENSY_OPT_SMALLEST_CODE_LTO``

The only one macro can be used in per one build environment. Also, you can see
verbose build using ``-v, --verbose`` option for :ref:`stm32` command.

Example:

Let's set optimization for the smallest code

.. code-block:: ini

    [env:teensy_hid_device]
    platform = teensy
    framework = arduino
    board = teensy36
    build_flags = -D TEENSY_OPT_SMALLEST_CODE

USB Features
~~~~~~~~~~~~

If you want to use Teensy USB Features, you need to add special macro/define
using :ref:`riscv`:

* ``-D USB_SERIAL``
* ``-D USB_KEYBOARDONLY``
* ``-D USB_TOUCHSCREEN``
* ``-D USB_HID_TOUCHSCREEN``
* ``-D USB_HID``
* ``-D USB_SERIAL_HID``
* ``-D USB_MIDI``
* ``-D USB_MIDI4``
* ``-D USB_MIDI16``
* ``-D USB_MIDI_SERIAL``
* ``-D USB_MIDI4_SERIAL``
* ``-D USB_MIDI16_SERIAL``
* ``-D USB_AUDIO``
* ``-D USB_MIDI_AUDIO_SERIAL``
* ``-D USB_MIDI16_AUDIO_SERIAL``
* ``-D USB_MTPDISK``
* ``-D USB_RAWHID``
* ``-D USB_FLIGHTSIM``
* ``-D USB_FLIGHTSIM_JOYSTICK``
* ``-D USB_EVERYTHING``
* ``-D USB_DISABLED``

A default macro is set to ``-D USB_SERIAL`` if no one is specified.

Example:

.. code-block:: ini

    [env:teensy_hid_device]
    platform = teensy
    framework = arduino
    board = teensy20
    build_flags = -D USB_RAWHID

See `Teensy USB Examples <https://www.pjrc.com/teensy/usb_debug_only.html>`_.

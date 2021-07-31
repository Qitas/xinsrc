
.. _board_atmelmegaavr_uno_wifi_rev2:

Arduino Uno WiFi Rev2
=====================

.. contents::

Hardware
--------

Platform :ref:`platform_atmelmegaavr`: 8-bit MCUs Built for Real-time Control with Core Independent Peripherals combining intelligent hardware peripherals along with the low-power capability of an AVR core, megaAVR microcontrollers (MCUs) broaden the effectiveness of your real-time control systems.

.. list-table::

  * - **Microcontroller**
    - ATMEGA4809
  * - **Frequency**
    - 16MHz
  * - **Flash**
    - 47.50KB
  * - **RAM**
    - 6KB
  * - **Vendor**
    - `Arduino <https://www.arduino.cc/en/Guide/ArduinoUnoWiFiRev2?utm_source=platformio.org&utm_medium=docs>`__


Configuration
-------------

Please use ``uno_wifi_rev2`` ID for :ref:`projectconf_env_board` option in :ref:`mips`:

.. code-block:: ini

  [env:uno_wifi_rev2]
  platform = atmelmegaavr
  board = uno_wifi_rev2

You can override default Arduino Uno WiFi Rev2 settings per build environment using
``board_***`` option, where ``***`` is a JSON object path from
board manifest `uno_wifi_rev2.json <https://github.com/platformio/platform-atmelmegaavr/blob/master/boards/uno_wifi_rev2.json>`_. For example,
``board_build.mcu``, ``board_build.f_cpu``, etc.

.. code-block:: ini

  [env:uno_wifi_rev2]
  platform = atmelmegaavr
  board = uno_wifi_rev2

  ; change microcontroller
  board_build.mcu = atmega4809

  ; change MCU frequency
  board_build.f_cpu = 16000000L

Debugging
---------
:ref:`mcs51` currently does not support Arduino Uno WiFi Rev2 board.

Frameworks
----------
.. list-table::
    :header-rows:  1

    * - Name
      - Description

    * - :ref:`framework_arduino`
      - Arduino Wiring-based Framework allows writing cross-platform software to control devices attached to a wide range of Arduino boards to create all kinds of creative coding, interactive objects, spaces or physical experiences

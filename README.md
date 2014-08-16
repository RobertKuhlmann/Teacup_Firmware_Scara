Teacup_Firmware_Scara
=====================

Scara-version of the RepRap Teacup-project

Define SCARA_PRINTER in config_wrapper.h to activate scara-step-calcualitons.
scara.h contains some scraa-specific constants.

dda_maths contains modified conversions (new: scara_um_to_steps) from �m to 
steps for scara-type printers.

dda.c uses the new method instead of the cartesian versions (um_to_steps_x/y).

ThermistorTable.h is configured for two EPCOS_thermistors:
hot end: EPCOS B57560G104F
bed:EPCOS B5786S0104F

The second one has a limited range up to 155 �C, but that's enough for a 
heated bed.
  

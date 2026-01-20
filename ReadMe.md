# **ZAP-A-PAL** Programmer - A Variation on a 1987-era PAL Programmer

ZAP-A-PAL was a low cost 20-pin and 24-pin PAL programmer developed by Robert A. Freedman and published in the [January 1987 issue](</Docs/ZAP-A-PAL - January 1987 BYTE Magazine.pdf>) of BYTE Magazine. It accepts JEDEC files which define the fuse map. It only programs bipolar PALs and not security fuses nor CMOS programmable logic devices. It does not support AMD PALs which used a different programming method. A good overview of PALs and the methods by which the fuses are burned is contained in the BYTE magazine issue.

This repository documents a variation made of the design by RCL9 in May 1987. Robert Freedman's version was based on an IBM PC card which he sold upon request. RCL9 designed a version which would run on his Z80-based machines connected to his  **Universal I/O Port** documented at the [bottom of this page](https://github.com/rcl9/Cypher-Z80-68000-Single-Board-Computer----Expansion-Board).

<div style="text-align:center">
<img src="/Images/Img2.jpg" alt="" style="width:75%; height:auto;">
</div>

<div style="text-align:center">
<img src="/Images/Img3.jpg" alt="" style="width:75%; height:auto;">
</div>

## Supported Manufacturers

The programmer will handle MMI standard PALs with A, B, and D speed suffixes and -2 and -4 power suffixes. lt will also handle 
National Semiconductor and Texas Instruments PALs.

| Type | Manufacturer Name |
| :-----: | :---: |
| |
| MMI | Monolithic Memories Inc |
| MMI-B | MMI high speed PALs |
| NSC | National Semiconductor |
| TI | Texas Instruments |

## Supported PAL Types

The following 20 and 24 pin variations are supported:

| PAL Name | Num Pins | Num Fuzes |
| :-----: | :---: | :---: |
| | |
| 16L8| 	20| 2048 |
| 16R8| 	20| 2048 |
| 16R6| 	20| 2048 |
| 16R4| 	20| 2048 |
| 10H8| 	20| 2048 |
| 10L8| 	20| 2048 |
| 12H6| 	20| 2048 |
| 12L6| 	20| 2048 |
| 14H4| 	20| 2048 |
| 14L4| 	20| 2048 |
| 16H2| 	20| 2048 |
| 16L2| 	20| 2048 |
| 16C1| 	20| 2048 |
| 16X4| 	20| 2048 |
| 16A4| 	20| 2048 |
| 12L10| 	24| 3200 |
| 14L8| 	24| 3200 |
| 16L6| 	24| 3200 |
| 18L4| 	24| 3200 |
| 20L2| 	24| 3200 |
| 20C1| 	24| 3200 |
| 20L10| 	24| 3200 |
| 20X10| 	24| 3200 |
| 20X8| 	24| 3200 |
| 20X4| 	24| 3200 |
| 20L8| 	24| 3200 |
| 20R8| 	24| 3200 |
| 20R6| 	24| 3200 |
| 20R4| 	24| 3200 |

## Source Code

The Z80 Aztec-C source code is contained in the [Src](/Src) directory.

## Schematics - Voltage Generators

<div style="text-align:center">
<img src="/Schematics/Schematics - 1.jpg" alt="">
</div>

<div style="text-align:center">
<img src="/Schematics/Schematics - 2.jpg" alt="">
</div>

## Schematics - Pin Drivers

<div style="text-align:center">
<img src="/Schematics/Schematics - 3.jpg" alt="">
</div>

## Schematics - DIP Component Platform Pinouts

<div style="text-align:center">
<img src="/Schematics/Schematics - 8.jpg" alt="">
</div>

## Schematics - Atari|ST 'Atari Bus' Interfaces

<div style="text-align:center">
<img src="/Schematics/Schematics - 4.jpg" alt="">
</div>

<div style="text-align:center">
<img src="/Schematics/Schematics - 5.jpg" alt="">
</div>

<div style="text-align:center">
<img src="/Schematics/Schematics - 7.jpg" alt="">
</div>

## Schematics - Changes and I/O Port Mapping

<div style="text-align:center">
<img src="/Schematics/Schematics - 6.jpg" alt="">
</div>

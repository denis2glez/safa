# Simple Aorta Flow Application (SAFA)

> This project was part of my undergraduate thesis in 2012, while this repository serves only as a
> historical archive and will never be updated.

## Abstract
*This work proposes a method of automatic segmentation of the aorta from 4D images (three spatial
dimensions joined to the temporal one) and a procedure for the centerline extraction of the same.
For this fundamental object, a specific segmentation pipeline is designed and implemented in which
several filters or procedures are applied sequentially. In order to subsequently incorporate major
visual information about the hemodynamic behavior, different methods of blood flow representation
are analyzed. The procedure used allows the medical expert to handle dynamic characteristics of
blood flow in the diagnosis of diseases inherent to this artery, and not base their criteria only
on the traditional analysis of the dimensions of the aorta, since this parameter is neither
determining nor robust.*

## Prerequisites

- [CMake](https://cmake.org/) build system.
- C++ compiler (tested on GCC and MSVC).
- [ITK](https://itk.org/) (Insight Toolkit).
- [VTK](https://vtk.org/) (Visualization Toolkit).
- [Qt](https://www.qt.io/) toolkit.

## Comments

It's been maybe around 8 years since I last took a look at this code, in fact I wasn't sure I had a
copy. While on the one hand I'm sure it must have quite a few aspects to be ashamed of right
now, it was a period in which I had the pleasure of working with several friends on a daily basis.
Finally, I have no hope that the project will compile today, given the years that have passed.
From the top of my head, I know that most of the development dependencies have received several
major releases since then.

## A picture is worth a thousand words

<div align="center">
 <img src="https://user-images.githubusercontent.com/30119590/140289326-f8c3c82a-f0ec-40bd-a48f-235f572560af.png"></a>
 <p>Figure 1: Centreline refinements from my bachelor’s thesis in 2012(<i>Segmentation of the aorta
 using cardiovascular 4D Magnetic Resonance Images</i>)</p>
</div>


<div align="center">
 <img src="https://user-images.githubusercontent.com/30119590/140289374-3c300e82-9f51-4b40-b511-a2513502043f.png"></a>
 <p>Figure 2: Example of segmentation obtained with this post-processing contribution.</p>
</div>
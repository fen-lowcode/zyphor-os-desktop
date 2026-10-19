#!/bin/sh

cp core/v2/_iso_custom/grub.cfg iso/boot/grub/grub.cfg
cp core/v2/_iso_custom/live.cfg iso/isolinux/live.cfg
cp core/v2/_iso_custom/splash-small.png iso/isolinux/splash.png
cp core/v2/_iso_custom/splash.png iso/boot/grub

echo "done".
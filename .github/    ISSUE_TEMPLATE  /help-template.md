---
name: Help form
about: Questions about this project
title: "[Help] "
labels: 'question'
assignees: ''

---

Exact **OS NAME** and **OS VERSION**: 
<br /><sub>I.e. "NAME" and "VERSION" in `/etc/os-release`, or "PRETTY_NAME" in `/etc/os-release`,
or via GUI e.g. on SailfishOS *Settings&nbsp;→ About&nbsp;product&nbsp;→ \<first line\>&nbsp;&&nbsp;Build*</sub>

**HARDWARE**:
<br /><sub>At least the output of `uname -m` or `uname -srm`, but preferrably full manufacturer and product name,
e.g. via GUI on SailfishOS *Settings&nbsp;→ About&nbsp;product&nbsp;→ Manufacturer&nbsp;&&nbsp;Product&nbsp;name*</sub>

**MeeCast VERSION**: 
<br /><sub>I.e. on MeeCast's *About* page at the very top (underneath the weather icon) in tiny font size,
<br />e.g. on SailfishOS open *MeeCast → [Top pulley] About*</sub>

Installed **MeeCast PACKAGES**: 
<br /><sub>I.e. the output of either `pkcon search name meecast | grep -Fi installed`,
<br />`rpm -qa | grep -F meecast`, `dpkg-query -l '*meecast*' | grep '^.[^n]'`
<br />or `apt list --installed '*meecast*'`</sub>

**SOURCE of MeeCast PACKAGES**: 
<br /><sub>Please denote, where you obtained the MeeCast packages from and how you installed them, e.g. on SailfishOS *"from OpenRepos via Storeman"*,
*"from SailfishOS:Chum via SailfishOS:Chum GUI app"*, *"manually downloaded from XYZ and installed by `pkcon install-local`"* etc.</sub>


#### QUESTION


#### STEPS TO REPRODUCE


#### ADDITIONAL INFORMATION

<sub>Please consider which other pieces of information may be relevant: Denote if this is not always reproducible,
if this is a regression (then name to which older version), attach relevant data such as log files or the systemd journal, provide screenshots etc.</sub>

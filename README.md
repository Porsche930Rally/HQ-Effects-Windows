# HQ Effects for Windows 1.0 Preview

Windows 10 / 11 **x64**. Native Win32 control app and C++20 DSP. This is a desktop port, not the Android USB helper. No Android phone or ADB is needed.

## Install

1. Run **HQEffects-Windows-1.0-Setup.exe**. It includes the official, unchanged Equalizer APO 1.4.2 x64 installer.
2. In the audio component's Configurator, enable your **playback** headphones/speakers. Do not select microphones. Reboot if requested.
3. Open **HQ Effects**, play music at a low initial listening level, then press **ON**. If setup has not connected HQ, use **Audio setup → Attach HQ** once.
4. Keep Windows audio enhancements enabled. Use stereo output and shared-mode audio. Exclusive WASAPI, ASIO, raw, and unsupported/offloaded paths can bypass the Windows effects chain.

The component installs at the output endpoint, so Spotify and other shared-mode apps use the same HQ effects. There is no capture/replay route or duplicate dry signal. Bluetooth, USB, wired and speaker outputs each need selection in the Configurator. Attachment is not proof that a particular device is processing: the footer reports audio callbacks received by HQ. Test your own devices.

## Tune

- Choose Atmosphere X-8 Deep, Clear, Acoustic / Vocal, or Flat and click **Load**.
- Every continuous control has a slider and an editable number. Type a value, then Enter, Tab, or click away to apply. Amounts use **0-1**, so `0.76` means 76%; gains are dB; periods are seconds; cutoffs are Hz.
- 31 EQ bands, left/right trim, IR wet/dry and output gain, bass/loudness, independent 8D speed/travel, ITD depth/position, measured elevation/front-back, stereo width, crossover and high/low-pass controls are included.
- Reverb choices contain the five original HQ WAV rooms. Import WAV/AIFF copies it permanently into HQ's data folder. Imports are limited to 16 MB, with at most the first five seconds decoded and used. Unsupported compressed files are rejected. Each loaded room is resampled and gain-normalized on a background thread.
- Type a user preset name and **Save / overwrite**. Load it from the same list, or **Delete** it. User presets include IR selection and every tuning control. Loading preserves the current ON/OFF state.
- **OFF** fades to dry over approximately 12 ms and releases the heavy engine. Bypass preserves the same 384-sample delay (8 ms at 48 kHz); the underlying Windows output buffer adds its normal latency. Closing the control app retains your last ON/OFF setting, so audio continues. Presets never turn effects on by themselves.
- The linked lookahead peak limiter protects the processed output at 0.90 sample peak. It is always enabled in this preview. Windows volume remains your output-device volume; HQ does not change it.

## Low memory design

No browser runtime, Electron, JVM, background capture or duplicated audio stream. The GUI writes small text settings; the DLL receives audio directly in the Windows effects host. DSP buffers and IR plans are preallocated outside audio callbacks. The loader checks configuration at 5 Hz and writes diagnostic telemetry at most once per second. Reverb/spectral histories are released after OFF fades out; rebuilding ON/IR changes happens in the background. The GUI footer shows its own private RAM, not the entire `audiodg.exe` process. DSP usage depends on sample rate and room length; see VALIDATION.md for measurements.

## Removal and troubleshooting

**Audio setup → Detach HQ** removes only HQ's marked configuration block. Windows Settings → Apps → HQ Effects uninstalls it and disables processing. A reboot may be needed to unload its DLL. User presets/imports remain in `%ProgramData%\HQEffects`; Equalizer APO remains separately installed.

Setup preserves the existing APO config, adds a marked HQ block, and saves a first-use `.hq-backup`. Other EQ/APO entries still run, so remove conflicting processing in that product's editor if you intentionally want HQ alone. Run device selection again if an output/driver changes. See `%ProgramData%\HQEffects\engine-status.txt` for the engine report. If callbacks are absent, check the selected playback device, enhancements and shared mode. This preview is not code-signed and has not yet been listening-tested on the user's Windows hardware.

Source is GPL-3.0-or-later. `build.sh` cross-compiles x64 with MinGW-w64; `build.cmd` builds using an installed MinGW-w64 toolchain and NSIS. Both require the included official audio-component installer at `dist/EqualizerAPO-x64-1.4.2.exe`. No proprietary VST SDK is included.

# Filter Designer

FIR and IIR low/high pass digital filter designer tool

* Low and high pass filters
* Finite and infinite impulse responses
* Various window types
* Frequency and phase analysis
* Pass and transition bands indication
* Free and open-source

<a href="https://apps.apple.com/us/app/digital-filter-designer/id6503596949?mt=12&amp;itsct=apps_box_badge&amp;itscg=30200" style="display: inline-block; overflow: hidden; border-radius: 13px; width: 250px; height: 83px;"><img src="https://tools.applemediaservices.com/api/badges/download-on-the-mac-app-store/black/en-us?size=250x83&amp;releaseDate=1717372800" alt="Download on the Mac App Store" style="border-radius: 13px; width: 250px; height: 83px;"></a>

![ui](https://github.com/frolovilya/filter-designer/assets/271293/1d399ffc-a78e-48a5-9b02-a3604fb8d41b)

## Usage

### Finite Impulse Response

Using window method for FIR filter design. 
Filter cofficients are generated by applying an inverse FFT on the ideal frequency response and then multiplying with a selected window function.

High pass filter is calculated from a low pass filter by shifting (multiplying) result coefficients with a sine wave of `pi/2` frequency sampled at `samplingRate/2`.

Number of filter coefficients is either calculated based on the target Attenuation (dB) and Transition Length (Hz) or entered manually.
Using odd number of coefficients allows for a linear phase response.

Frequency and phase responses are calculated with a direct FFT of the filter coefficients.

Filter coefficients `c[k]` must be convolved with a sample buffer to apply a FIR filter:

```
Vout[i] = c[k] * Vin[i-k] + c[k-1] * Vin[i-(k-1)] + ... + c[0] * Vin[i]
```

Where `Vin[]` is a input circle sample buffer of size `n`, `k` is a total number of filter coefficients, `Vin[i]` is a last input sample, `Vout[i]` is a current output filtered sample.


### Infinite Impulse Response

Basic analog filter replication. Using RC circuit for a low-pass and CR circuit for a high-pass filters.
![analog_filters](https://github.com/frolovilya/filter-designer/assets/271293/bb6708b6-c6e0-46e5-94ad-2fad4b31665e)

Frequency and phase responses are calculated by simulating sine waves of various frequencies and applying IIR filters directly.

IIR filter with coefficients `c[3]` must be applied the following way:

```
Vout[i] = c[0] * Vin[i] + c[1] * Vin[i-1] + c[2] * Vout[i-1]
```

Where `Vin[i]` is a current input sample, `Vout[i]` current output filtered sample, `Vin[i-1]` and `Vout[i-1]` are previous input and output samples used as a feedback for the IIR filter.



## Build

The project depends on [Qt](https://doc.qt.io/qt-6/get-and-install-qt.html), [FFTW3](https://www.fftw.org/download.html), [Welle](https://github.com/frolovilya/Welle) and [Boost](https://www.boost.org/) libraries.

Qt path is passed to the CMake build via `-DQT_PATH` option, since you may wish to install it via Qt Maintenance Tool to any directory.

FFTW3, Welle and Boost must be available in you _/usr/local/include_ for CMake to resolve the paths.

```
mkdir build; cd build
cmake ../ -DQT_PATH=/Users/frolovilya/Dev/tools/QT/6.7.0/macos

make
```



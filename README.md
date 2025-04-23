# 🖼️ BMP Image Processing in C

This project is a complete BMP image processing program written in C.  
It supports both **8-bit grayscale** and **24-bit RGB** uncompressed BMP files.

The program provides a **text-based interface** that allows users to open, analyze, apply filters, and save modified images.

---

## Project Overview

- Language: **C**
- Supported formats: **BMP 8-bit** and **BMP 24-bit**
- Terminal-based interface
- Compilation: `gcc main.c bmp8.c bmp24.c -o bmpfilter -lm`
- Filters included:
    - Negative
    - Brightness adjustment
    - Threshold (8-bit)
    - Grayscale (24-bit)
    - Box Blur
    - Gaussian Blur
    - Outline
    - Emboss
    - Sharpen

---

# PARTIE 1 – 8-bit BMP (Grayscale)

## Files
- `bmp8.h` — Header for grayscale image structure and functions
- `bmp8.c` — Image loading, saving, and filters for 8-bit images

## Features (Part 1)
- Load BMP 8-bit uncompressed image
- Display width, height, depth
- Apply:
    - Negative
    - Brightness
    - Threshold (black and white)
    - Box Blur
    - Gaussian Blur
    - Outline
    - Emboss
    - Sharpen
- Save image back to `.bmp`

---

# PARTIE 2 – 24-bit BMP (Color)

## Files
- `bmp24.h` — Header for color image structure and functions
- `bmp24.c` — Image loading, saving, and filters for 24-bit images

## Features (Part 2)
- Detect BMP format (8-bit or 24-bit)
- Support BMP 24-bit uncompressed files
- Display image info
- Apply filters:
    - Negative
    - Grayscale
    - Brightness
    - Box Blur
    - Gaussian Blur
    - Outline
    - Emboss
    - Sharpen

---

## Interface

```text
Please choose an option:
1. Open an image
2. Save image
3. Apply a filter
4. Display image information
5. Quit
```

# PARTIE 3 – (soon)
...
# BONUS

You can find in the zip file, another folder named `Projet_interface`  
### **May sure to open ALL files separately that the other** 
1. Open the `Projet_interface` folder in CLion (or use CMake manually).
2. Make sure SDL2 and SDL2_ttf libraries are accessible.
3. Run `interface.c` from CLion.


```bash
  Bramli, Gastaldo INT4 Project/
├──   modules/                
│   ├── /SDL2_ttf-2.20.2
│   ├── /SDL2-2.30.12
├── Projet_C
│   ├── ...
├── Projet_interface
│   ├── /cmake-build-debug
│   ├── /bmp8.c
│   ├── /bmp8.h
│   ├── /bmp24.c
│   ├── /bmp24.h
│   ├── /CmakeLists.txt
│   ├── /main.c
└──
```

- Also **Language:** C (C99)
- New **Libraries:**
    - **SDL2** (**SDL2-2.30.12** in the module file) – for window management and rendering
    - **SDL2_ttf** (**SDL2_ttf-2.20.2** in the module file) – for rendering fonts and text

## Project Structure

| File             | Description                             |
|------------------|-----------------------------------------|
| `interface.c`    | SDL2 graphical interface + menu logic   |
| `bmp8.c/h`       | Image loading + filters for 8-bit BMP   |
| `bmp24.c/h`      | Image loading + filters for 24-bit BMP  |
| `CMakeLists.txt` | Project configuration for CLion / CMake |

## In `interface.c` :

## ️ Controls

| Action                | Key           |
|-----------------------|---------------|
| Navigate              | ↑ / ↓         |
| Select menu item      | Enter         |
| Confirm console input | Space         |
| Exit                  | ESC or "Quit" |

### **On the screen we have the same menu as the original project on the screen**

## In `bmp8.c/h` and `bmp24.c/h` :

### **Same files I have just copied and pasted that I made previously** 

## In `CMakeLists.txt` : 

 **Important to don't touch the configuration because they are the new library (SDL2)**
 ## **BUT MODIFY THIS UNDERLINE PATH BY YOUR PATH**
  ![modify](./screenshots/modify.png)
  # not necessary to change the not underline path 
## Notes

- Make sure `SDL_ttf` is installed and `SDL2_ttf.dll` is in the executable directory:  
  (Verify but those file must be present in the `CMakelist.txt` folder)
- The font used is: `C:/Windows/Fonts/arial.ttf` (can be changed)
- File paths must be entered via **console input**, not SDL window  


   ![path](./screenshots/path.png)  
  *Example*

## Information

During holidays, we follow some videos to create graphic interface.  
I watch this :  
  [video 1]("https://www.youtube.com/watch?v=yX05p3FUmEA")    
  [video 2]("https://www.youtube.com/watch?v=Lwx9rSgwoDg&t=1460s")   
  [video 3]("https://www.youtube.com/watch?v=N5CZLSVU0DA&t=407s")  
And I use CHATGPT to help me in the process of the main of the interface version







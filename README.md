### Pure C++ Text Editor
A Modern-ish Text Editor using purely C++ and WinApi

## Features
* Implemented a Document->Page->column->row data structure.
* **Manual Rendering:** Uses GDI (`TextOutW`) to draw text
* **Spell checker**:-
                    binary search validation against an external CSV dictionary
* Search and Highlight.
* **Auto saving every 60 seconds**
                    Integrated background backup logic that triggers every 60 seconds (Timer ID: 2007).
* The ability to Control the number of columns, rows and charachters per rows.
* **Text Alignment**
* **Word Integrity across lines**
* **Automatic Table of Contents generation.**
* **Live Stats:-**
               Real-time metrics for characters, words, sentences, and estimated reading time.
* Supports ten Concurrent Documents.


## KeyBoard Shorcuts to know

| Combination | Action |
| :--- | :--- |
|Ctrl + L / E / R | Align Text Left / Center / Right |
|Ctrl + J | Justify Text |
|Ctrl + F | Toggle Search Mode |
|Ctrl + T | Toggle Table of Contents |
|Ctrl + Tab | Move Forward one Document |
|Ctrl + Shift + Tab| Move backwards one Document |

## 🛠️ Build & Setup Guidelines
### Prerequisites
* **Windows OS** (WinAPI Dependency).
* **Visual Studio 2022** (or C++ compiler with Windows SDK).
* **Dictionary File:** `OPTED-Dictionary.csv` must be in the executable directory for spell-check functionality.

### Build Steps
1. Open the source files (`Q7_Main.cpp` and `Q7_Submission.h`) in a **Windows Desktop Application** project.
2. Ensure the Character Set is set to **Unicode** in Project Properties.
3. Compile for **x64** or **x86**.


## Credits ❤️
# Made by Hammad Rehan
2nd Semester CS Student, FAST NUCES ISB

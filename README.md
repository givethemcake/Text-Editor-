### Pure C++ Text Editor
This project is a high-performance, lightweight text editor built from the ground up using **C++** and the **Windows API**. Moving away from standard Windows Edit controls, this implementation features a manual rendering engine and a hierarchical Document Object Model to handle complex layouts and paging.

## Features
* **Hierarchical Document Engine:** Implemented a custom-engineered `Document > Page > Column > Line` data structure for precise memory and text management.
* **Manual GDI Rendering:** Bypasses standard controls to draw text directly via `TextOutW` and `GetTextExtentPoint32W` for pixel-perfect placement.
* **Intelligent Text Processing:**
    * **Advanced Spell Checker:** High-performance $O(\log n)$ binary search validation against an external CSV dictionary.
    * **Word Integrity (Smart Wrapping):** Custom logic ensures words remain intact and do not break awkwardly across lines or columns.
    * **Dynamic Alignment:** Native support for Left, Center, Right, and **Full Justification** (using pixel-distributed spacing).
* **Automation & Safety:**
    * **Birth-Year Auto-Save:** Background backup logic (Timer ID: 2007) that secures progress every 60 seconds.
    * **Smart Table of Contents:** Automatically generates a TOC by analyzing "header-like" patterns and word frequency at the start of pages.
* **Analytics & Multitasking:**
    * **Live Statistics Dashboard:** Real-time metrics for characters (with/without spaces), words, sentences, and **Estimated Reading Time**.
    * **Multi-Document Support:** Capability to manage up to **10 concurrent documents** in memory with fast switching.
    * **Search & Highlight:** Integrated search functionality with visual background highlighting for matches.


## KeyBoard Shorcuts to know

| Combination | Action |
| :--- | :--- |
|Ctrl + L / E / R | Align Text Left / Center / Right |
|Ctrl + J | Justify Text |
|Ctrl + F | Toggle Search Mode |
|Ctrl + T | Toggle Table of Contents |
|Ctrl + Tab | Move Forward one Document |
|Ctrl + Shift + Tab| Move backwards one Document |

## Build & Setup Guidelines
### Prerequisites
* **Windows OS** (WinAPI Dependency).
* **Visual Studio 2022** (or C++ compiler with Windows SDK).
* **Dictionary File:** `OPTED-Dictionary.csv` must be in the executable directory for spell-check functionality.

### Build Steps
1. Open the source files in a **Windows Desktop Application** project.
2. Ensure the Character Set is set to **Unicode** in Project Properties.
3. Compile for **x64** or **x86**.


## Credits ❤️
# Made by Hammad Rehan
2nd Semester CS Student, FAST NUCES ISB

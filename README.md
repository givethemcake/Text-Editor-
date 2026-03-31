### Pure C++ Text Editor
This project is a high-performance, lightweight text editor built from the ground up using **C++** and the **Windows API**. Moving away from standard Windows Edit controls, this implementation features a manual rendering engine and a hierarchical Document Object Model to handle complex layouts and paging.

## Technical Highlights
* **Hierarchical Document Engine:** Implemented a custom-engineered `Document > Page > Column > Line` data structure for precise memory and text management.
* **Manual GDI Rendering:** Bypasses standard controls to draw text directly via `TextOutW` and `GetTextExtentPoint32W` for pixel-perfect placement.
* **Optimized Spell Checker:** Features high-performance $O(\log n)$ binary search validation against an external CSV dictionary.
* **Smart Word Integrity:** Custom wrapping logic ensures words remain intact and do not break awkwardly across lines or columns.
* **Dynamic Alignment Engine:** Native support for Left, Center, Right, and **Full Justification** using manual pixel-distribution spacing.

  <img width="1919" height="1005" alt="image" src="https://github.com/user-attachments/assets/9e8434b1-8061-4132-aef4-4e1dc5091d51" />


## Key Features 
* **Automation & Safety:** A background backup logic (Timer ID: 2007) that secures progress every 60 seconds.
* **Live Statistics Dashboard:** Real-time metrics for characters (with/without spaces), words, sentences, and **Estimated Reading Time**.
* **Intelligent Navigation:** Automatic **Table of Contents (TOC)** generation based on page content analysis and "header-like" pattern detection.
* **Multi-Document Multitasking:** Capability to manage up to **10 concurrent documents** in memory with integrated search and visual match highlighting.
* **Search & Highlight:** Integrated search functionality with visual background highlighting for matches.
 <img width="1328" height="485" alt="image" src="https://github.com/user-attachments/assets/df12aedf-752e-40e1-9b4d-691132196dd6" />


## KeyBoard Shorcuts to know

| Combination | Action |
| :--- | :--- |
|Ctrl + L / E / R | Align Text Left / Center / Right |
|Ctrl + J | Justify Text |
|Ctrl + F | Toggle Search Mode |
|Ctrl + T | Toggle Table of Contents |
|Ctrl + Tab | Move Forward one Document |
|Ctrl + Shift + Tab| Move backwards one Document |
|Ctrl + ESC| Exit|

## Build & Setup Guidelines
### Prerequisites
* **Windows OS** (WinAPI Dependency).
* **Visual Studio 2022** (or C++ compiler with Windows SDK).
* **Dictionary File:** `OPTED-Dictionary.csv` must be in the executable directory for spell-check functionality.

### Build Steps
1. Open the source files in a **Windows Desktop Application** project.
2. Ensure the Character Set is set to **Unicode** in Project Properties.
3. Compile for **x64** or **x86**.

<img width="1401" height="669" alt="image" src="https://github.com/user-attachments/assets/326a7d1c-7e74-47f6-a8be-acb7f185adef" />


## Credits ❤️
# Made by Hammad Rehan
2nd Semester CS Student, FAST NUCES ISB

# PyQt6 UI
# Created: 12.07.2023 12:59:00
# Author: Aleksey M.
# Tested: YES
# ----------------------------
# pip install pyqt6
# pip install pyqt-tools
# Run: python app.py

""" ---------- App Example ---------- """

from PyQt6.QtWidgets import QApplication, QWidget
import sys 

app = QApplication(sys.argv)  # Create new instance of QApplication
window = QWidget()            # Create new instance of QWidget
window.setWindowTitle("PyQT6 UI");
window.show()

app.exec()
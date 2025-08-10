import tkinter as tk
from tkinter import ttk, filedialog, scrolledtext
from serial.tools import list_ports
import logging
import threading
from app.main import run

class LogHandler(logging.Handler):
    """Кастомный обработчик логов для Tkinter"""
    def __init__(self, text_widget):
        super().__init__()
        self.text_widget = text_widget
        self.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))

    def emit(self, record):
        msg = self.format(record)
        self.text_widget.after(0, self._append_log, msg, record.levelname.lower())

    def _append_log(self, msg, level):
        self.text_widget.configure(state="normal")
        self.text_widget.insert(tk.END, msg + "\n", level)
        self.text_widget.configure(state="disabled")
        self.text_widget.see(tk.END)

class App:
    def __init__(self, root):
        self.root = root
        self.setup_ui()
        self.setup_logging()
        self.running = False
        self.worker_thread = None

    def setup_ui(self):
        self.root.title("COM Port + File Selector")
        self.root.geometry("800x600")

        # Панель параметров
        frame = ttk.Frame(self.root)
        frame.pack(pady=10, fill="x")

        # COM-порт
        ttk.Label(frame, text="COM Port:").grid(row=0, column=0, padx=5)
        self.combo_com = ttk.Combobox(frame, values=get_com_ports())
        self.combo_com.grid(row=0, column=1, padx=5)
        if self.combo_com['values']:
            self.combo_com.current(0)

        # Файл
        ttk.Label(frame, text="TXT File:").grid(row=1, column=0, padx=5)
        self.entry_file = ttk.Entry(frame, width=40)
        self.entry_file.grid(row=1, column=1, padx=5)
        ttk.Button(frame, text="Browse...", command=self.select_file).grid(row=1, column=2)

        # Кнопки управления
        self.btn_run = ttk.Button(self.root, text="Run", command=self.start_thread)
        self.btn_run.pack(pady=10)
        # Лог
        self.log_area = scrolledtext.ScrolledText(
            self.root,
            wrap=tk.WORD,
            state="disabled",
            height=20
        )
        self.log_area.pack(fill="both", expand=True, padx=10, pady=10)

        # Цвета логов
        self.log_area.tag_config("info", foreground="black")
        self.log_area.tag_config("warning", foreground="orange")
        self.log_area.tag_config("error", foreground="red")
        self.log_area.tag_config("debug", foreground="gray")

    def setup_logging(self):
        self.log_handler = LogHandler(self.log_area)
        logging.basicConfig(level=logging.INFO, handlers=[self.log_handler])

    def select_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
        if file_path:
            self.entry_file.delete(0, tk.END)
            self.entry_file.insert(0, file_path)

    def start_thread(self):
        if self.running:
            return

        com_port = self.combo_com.get()
        file_path = self.entry_file.get()

        if not com_port or not file_path:
            logging.error("Выбери COM порт и Путь к файлу программы")
            return

        self.running = True
        self.btn_run.config(state="disabled")

        self.worker_thread = threading.Thread(
            target=self.run_main_app,
            args=(com_port, file_path)
        )
        self.worker_thread.start()

    def run_main_app(self, com_port, file_path):
        try:
            run(com_port, 9600, file_path)
        except Exception as e:
            logging.error(f"Error in main app: {str(e)}")
        finally:
            self.running = False
            self.root.after(0, self.on_thread_finished)

    def on_thread_finished(self):
        self.btn_run.config(state="normal")


def get_com_ports():
    return [port.device for port in list_ports.comports()]


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
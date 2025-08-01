\# Smart Lab PSU — Умный лабораторный блок питания



\*\*Проект умного лабораторного блока питания на базе ESP32-S2\*\*  

Гибридная схема: аналоговая надёжность + цифровое управление и Web-интерфейс.



---



\## 📋 Содержание



\- \[Описание проекта](#описание-проекта)

\- \[Характеристики](#характеристики)

\- \[Архитектура и файлы](#архитектура-и-файлы)

\- \[Документация (ТЗ)](#документация-тз)

\- \[Схема и плата (EasyEDA)](#схема-и-плата-easyeda)

\- \[Сборка и отладка](#сборка-и-отладка)

\- \[Планы и IDE](#планы-и-иде)

\- \[Лицензия](#лицензия)



---



\## Описание проекта



Smart Lab PSU — лабораторный блок питания с ESP32‑S2, комбинирующий точность, безопасность и удобство: аналоговая архитектура, цифровые защиты, OTA и веб‑интерфейс.



---



\## Характеристики



\- Выходное напряжение: 1.25–36 В  

\- Максимальный ток: до 3 А  

\- Измерения: INA226 (ток и напряжение)  

\- Температурный контроль: NTC 10 к (B = 3470) с защитой  

\- Режимы: ручной и автоматический (PWM — шина обратной связи)  

\- Управление: экран + энкодер + веб-интерфейс + OTA



---



\## Архитектура и файлы



\- `main.ino` — точка входа  

\- `PowerControl.\*` — управление питанием и защита  

\- `Globals.\*`, `Config.h` — глобальные параметры и пины  

\- Остальные модули: DisplayManager, Ina226Manager, LbpWebInterface и др.  

\- 📄 `docs/chapters/` — ТЗ, документация



---



\## Документация (ТЗ)



Подробные главы в папке `docs/chapters`:



1\. Общая концепция  

2\. Архитектура  

3\. Безопасность  

4\. Ограничения  

5\. Экран  

6\. Веб‑интерфейс  

7\. PowerControl  

8\. Планы  

9\. Архив экспериментов



---



\## Схема и плата (EasyEDA)



Полная схема и дизайн PCB доступны на EasyEDA:  

➡️ https://oshwlab.com/dmy200/lbp\_pulse



---



\## Сборка и отладка



1\. `git clone https://github.com/dmy200/smart-lab-psu.git`  

2\. Открой `main.ino` в Arduino IDE или VSCode  

3\. Настрой Wi‑Fi в `Secrets.h`  

4\. Скомпилируй и залей на ESP32‑S2



Совет: держи проект синхронизированным через GitHub Desktop и не забудь `.gitignore`!



---



\## Планы и IDE



\- Развитие защит (ток, мощность)  

\- Поддержка Type‑C QC/PD  

\- Логи, профили, графики  

\- Используй GitHub Desktop — удобно и понятно



---



\## Лицензия



MIT License (подробности в `LICENSE`)



---






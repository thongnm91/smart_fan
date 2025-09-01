<img width="444" height="624" alt="image" src="https://github.com/user-attachments/assets/8eccc872-63bd-4faf-b144-53cd5218d123" /># Smart Fan Control with ATtiny2313

This project uses the **ATtiny2313 microcontroller**, combining **Fast PWM** and **external interrupts** to control a 4-wire fan. The idea is simple but effective:
[Demo video](https://www.youtube.com/shorts/Uym_zOKH5ic) 



## User Controls

- **Press the left button** to increase fan speed  
- **Press the right button** to decrease fan speed  
- **Max speed:** Right-side LED lights up  
- **Min speed:** Left-side LED turns on  

## Technical

- **Fast PWM on Timer0** generates the fan control signal  
- **INT0 and INT1** handle button inputs via external interrupts  
- **Debouncing** is done using **Timer1** directly inside the **Interrupt Service Routines (ISR)**  
- **Speed changes in steps**, with duty cycle limits checked to avoid overflow or underflow  
- **LED indicators** on **PB6** (min) and **PB7** (max) provide visual feedback  

---

This approach provides clean, responsive fan speed control using just a few lines of code and **no delay functions**. All logic is handled through **interrupts**, making it efficient and **power-friendly** — ideal for embedded systems with limited resources!

![](<img width="444" height="624" alt="image" src="https://github.com/user-attachments/assets/d9a577d7-78e5-4045-b24e-07bd865f2fa6" />)

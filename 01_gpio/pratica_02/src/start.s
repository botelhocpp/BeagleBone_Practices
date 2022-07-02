_start:
   mrs r0, cpsr
   bic r0, r0, #0x1F @ clear mode bits
   orr r0, r0, #0x13 @ set SVC mode
   orr r0, r0, #0xC0 @ disable FIQ and IRQ
   msr cpsr, r0

   ldr sp, =0x4030CDFC  @6kB public stack  TMR 26.1.3.2

   bl _main


.loop: b .loop


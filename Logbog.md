
https://prod.liveshare.vsengsaas.visualstudio.com/join?650B1EEC74F2484E932EC11CAF3821057152

**21/05** 
*11:40*
Vi startede med at få et led og en knap til at virke. Derefter Satte vi alle knapperne og led'erne op. Og satte hver knap til at tende en led i 1 sekund, for at teste at alle vores inputs virkede.
Vi opdelte derefter arbejdet, for at hurtigere indhente det vi er bagud fra igår.
En tog Wifi og NTP
Imens en anden lavede ButtonHandler klasse, til at stå for setup og håndtering af knappen og tilhørende LED. Dette inkludere løsning til debouncing & timeout.

*14:10*
Vi har opdelt NTP / tidshåndetering til sin egen klasse. Og begyndt på deep sleep implementeringen. Vi har opsat sådan det "virker", men mere en poc. Vi overvejer at gøre det til sin egen lille process / countdown. 
Der er flere der er gået udenom loop metoden, men jeg har svært ved at se hvordan vi skal håndtere debounce uden. Har fået lidt hjælp af Sebastian, og måske jeg ser på debounce som en ting med at lidt større timeout...
I morgen vil vi færdiggøre vores deep sleep og få implementeret MQTT, til data kald.


**22/05**
*11:20*
Vi har fået færdiggjort den første implementation af deepsleep hvor hele flowet virker, dog uden mqtt kald. Der er blevet lavet en del overvejelser om hvor meget der skulle hånderes i setup & i loop metoden. Men siden vores nuværende løsning ikke har brug for loop, så køre det hele i setup, uden debounce da der kun er et input (touch wakeup).
Næste step er at få implementeret og færdiggjort mqttHandler klassen.


*13:00*
Vi har siddet og lavet nogle små ændinger samt testet den på powerbanken. Vi har også arbejdet videre på mqtt klassen, men uden held for nu. Så vi er gået i gang med et lille refactor af klassen. Og den virker tæt på at virke.

**23/05**
*08:01*
Fik Mqtt til at virke! Så nu skal vi have sendt noget god data med så man nemt ved hvilken smiley der er trykket ud fra dataen. Efter vil vi drøfte hvad der kan optimeres.

*10:06*
Jeg har lavet noget oprydning i koden. forsøgte mig ad med at lave at klasse til at håndtere deep sleep. Det fungerede til at putte enheden i sleep. Men jeg havde nogle problemer med at få den til at vågne på touch. Tror det kunne have noget at gøre med pointeren til buttons som jeg passede ind til klassen fra main, som ikke var korrekt.
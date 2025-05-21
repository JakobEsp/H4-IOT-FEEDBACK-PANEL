
https://prod.liveshare.vsengsaas.visualstudio.com/join?650B1EEC74F2484E932EC11CAF3821057152

21/05 - 11:40
Vi startede med at få et led og en knap til at virke. Derefter Satte vi alle knapperne og led'erne op. Og satte hver knap til at tende en led i 1 sekund, for at teste at alle vores inputs virkede.
Vi opdelte derefter arbejdet, for at hurtigere indhente det vi er bagud fra igår.
En tog Wifi og NTP
Imens en anden lavede ButtonHandler klasse, til at stå for setup og håndtering af knappen og tilhørende LED. Dette inkludere løsning til debouncing & timeout.
21/05 - 14:10
Vi har opdelt NTP / tidshåndetering til sin egen klasse. Og begyndt på deep sleep implementeringen. Vi har opsat sådan det "virker", men mere en poc. Vi overvejer at gøre det til sin egen lille process / countdown. 
Der er flere der er gået udenom loop metoden, men jeg har svært ved at se hvordan vi skal håndtere debounce uden. Har fået lidt hjælp af Sebastian, og måske jeg ser på debounce som en ting med at lidt større timeout...
I morgen vil vi færdiggøre vores deep sleep og få implementeret MQTT, til data kald.
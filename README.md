# LunariaAdaptive
Konsolowy, adaptacyjny system analizy cyklu biologicznego i rekomendacji w C++  
## O projekcie  
Większość popularnych narzędzi ślepo zakłada uniwersalny, sztywny model cyklu, sprowadzając biologię do prostej matematyki kalendarzowej i całkowicie ignorując unikalność fizjologii każdego człowieka.
`LunariaAdaptive` wprowadza **podejście adaptacyjne oparte na obiektowej analizie historii użytkownika oraz modelowaniu profilu hormonalnego.**  
* **Adaptacyjność:** System oblicza parametry na podstawie realnie wprowadzanych danych. Im więcej cykli użytkownik wprowadzi do bazy, tym precyzyjniej program estymuje długość nadchodzących faz.  
* **Polimorficzny system rekomendacji:** Każda faza biologiczna oparta na profilu hormonalnym jest osobną klasą. Dzięki temu program w sposób dynamiczny generuje dedykowane wskazówki.

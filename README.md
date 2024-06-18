Feladat: Az alapvető koordinátageometriai ismeretek felelevenítése és az OpenGL-lel való
ismerkedés. Az elkészítendő programmal ízlésesen megválasztott szürke háttéren egérkattintásokkal
pontokat vehetünk fel, a pontokra egyeneseket illeszthetünk, az egyeneseket tologathatjuk, és végül a
kiválasztott egyenesek metszéspontjaiból újabb pontok hozhatók létre. A pontméret 10-es a
vonalvastagság 3.
A világ-koordinátarendszer megegyezik a normalizált eszköz-koordinátarendszerrel, azaz a keletkező
fénykép (viewport) a (-1,-1) és (1,1) sarokpontú négyzetnek felel meg. A fénykép pedig teljes mértékben
lefedi a 600x600 felbontású alkalmazói ablakot.
A CPU programban a 2D geometriát külső szemlélettel, azaz a 3D térbe ágyazva kell kezelni. Ez azt jelenti,
hogy a 2D pontokhoz és vektorokhoz vec3 típust rendelünk, pontokra a 𝑧 = 1, vektorokra a 𝑧 = 0
választással. Majd a csúcspont árnyalóban a 3D geometriát 4D-be ágyazzuk be 𝑤 = 1 választással.
Felhasználói oldalról a programnak 4 állapota van, amelyeket rendre a p, l, m, i billentyűkkel lehet
kiválasztani:
-‘p’: Pont rajzolás, amely az egér bal gombjának lenyomásakor a kurzor helyére egy maximális intenzitású
  piros pontot tesz.

-’l’: Egyenes rajzolás, amelyhez két meglévő piros pontra kell rákattintani az egér bal gombjával. Az
  egyenes maximális intenzitású cián (türkisz) színben pompázik.

-’m’: Egyenes eltolás, amelyhez először az egyenest kell kiválasztani az bal egérgombbal, és ha ez sikeres,
  az egyenes követi a kurzort, azaz az egér lenyomott gomb melletti mozgatását, mindaddig, amíg el
  nem engedjük az egérgombot.

-’i’: Metszéspont, amely két kiválasztott egyenes metszéspontjára (ha létezik) egy új piros pontot tesz.

A program a keletkező pontok Descartes koordinátáit, valamint a keletkező egyenesek implicit és
parametrikus egyenleteit printf-fel a konzolra kiírja.

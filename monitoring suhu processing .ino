

    /*Menampilkan suhu LM35 pada Processing
     * 14 Januari 2016
     * http://www.boarduino.web.id
    */

    void setup() {
      //Kecepatan komunikasi serial
      Serial.begin(9600);
    }

    void loop() {
      //Tampung sementara hasil A0 di variable int LM35
      int LM35 = analogRead(A0);
      //hasil LM35*0.48828125 (Untuk menghasilkan °C)
      int suhuCelcius = round(LM35*0.48828125);
      //hasil Celcius *9.0/5.0 + 32.0 ((Untuk menghasilkan °F)
      int suhuFahrenheit = round(suhuCelcius*9.0/5.0 + 32.0);

      Serial.print(suhuCelcius); //Print hasil suhu Celcius
      Serial.print("C");
      Serial.print(suhuFahrenheit); //Print hasil suhu Fahrenheit
      Serial.println("F");
      delay(500); //Delay setengah detik
    }


<!doctype html>
<html>
    <head>
        <title>Table with database</title>
        <link rel="stylesheet" href="style.css">
        <meta http-equiv="refresh" content="5">
        <!-- Script collegato alla libreria per la creazione del grafico a torta  -->
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    </head>
    <body>
        <!-- Creazione della tabella in cui vengono mostrati gli attuali valori del numero di figure rilevate -->
        <table id="FigTab">
            <thead>
                <tr>
                    <th>NumFigureOk</th>
                    <th>NumFigureCorte</th>
                    <th>NumFigureBasse</th>
                </tr>
            </thead>
            <tbody>
                <?php
                    //Connessione al database mysql
                    $conn = mysqli_connect("localhost","root","","iot");

                    //Gestione dell'errore di connessione
                    if($conn -> connect_error){
                        die('connection failed:'. $conn -> connect_error);
                    }

                    //Prendo l'utlima riga del database
                    $sql = "SELECT NumFigureOk,NumFigureCorte,NumFigureBasse from data ORDER BY NumFigureOk DESC LIMIT 1";
                    $result = $conn -> query($sql);

                    if($result -> num_rows > 0){
                        //Inserisco i dati presi dal database nella tabella HTML
                        while($row = $result -> fetch_assoc()){
                            echo "<tr><td id='OK'>".$row["NumFigureOk"]."</td><td id='Corte'>".$row["NumFigureCorte"]."</td><td id='Basse'>".$row["NumFigureBasse"]."</td></tr>";
                        }
                        echo "</table>";
                    }

                ?>
            </tbody>
        </table>

        <!-- Spazio in cui verrà inserita la percentuale di figure corrette, calcolata con javascript -->
        <p id="percentualeCorrette">
            
        </p>

        <!-- Spazio in cui verrà inserito il grafico a torta realizzato con javascript -->
        <div class='container'>
            <div>
                <canvas id="pie-chart"></canvas>
            </div>
        </div>

        <!-- script per la realizzazione del grafico a torta -->
        <script>

            //Estrazione dei numeri presenti in tabella delle figure rilevate per ciascuna categoria
            ok =  document.getElementById('OK').innerHTML;
            corte =  document.getElementById('Corte').innerHTML;
            basse =  document.getElementById('Basse').innerHTML;

            //Calcolo della percentuale di figure corrette e inserimento della relativa stringa nell'apposito paragrafo HTML
            tot = +ok + +corte + +basse;
            percentualeCorrette = Math.round((ok / tot)*100);

            if(percentualeCorrette > 0){
                document.getElementById('percentualeCorrette').innerHTML = "Percentuale di figure corrette: " + percentualeCorrette + "%.";
            }
            
            //Creazione del grafico a torta utilizzando Chart.js
            new Chart(document.getElementById('pie-chart'), {
                type: 'pie',
                data: {
                    labels: ["Figure Corrette", "Figure Corte", "Figure Basse"],
                    datasets: [{
                        backgroundColor: ["#33FF33", "#FF9933","#FF3333"],
                        data: [ok, corte, basse]
                    }]
                },
                options: {
                    title: {
                        display: true,
                        text: 'Pie Chart for admin panel'
                    },
                    responsive: true
                }
            });
 
        </script>

        <!-- pulsante per l'eliminazione dei dati presenti nel database -->
        <form method="POST">
            <input type="submit" value="RESETTA DATI" id="buttonReset">
        </form>

        <?php
            if ($_SERVER['REQUEST_METHOD'] == 'POST') {
                $sql = "DELETE FROM data";

                if ($conn->query($sql) === TRUE) {
                    echo "Tutte le righe sono state eliminate con successo.";
                } else {
                    echo "Errore durante l'eliminazione: " . $conn->error;
                }
            }
        ?>
    </body>
</html>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Web Page</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
            background: linear-gradient(135deg, #2c3e50, #4ca1af);
            color: #fff;
            animation: fadeIn 1s ease-in-out;
        }

        .container {
            text-align: center;
            max-width: 600px;
            padding: 20px;
            background: rgba(0, 0, 0, 0.2);
            border-radius: 10px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.2);
            animation: slideUp 0.8s ease-in-out;
        }

        h1 {
            font-size: 3rem;
            margin-bottom: 20px;
            color: #f1c40f;
        }

        a {
            display: inline-block;
            padding: 10px 20px;
            font-size: 1.2rem;
            color: #fff;
            text-decoration: none;
            border: 2px solid #f1c40f;
            border-radius: 5px;
            transition: all 0.3s ease;
        }

        a:hover {
            background-color: #f1c40f;
            color: #2c3e50;
        }

        /* Page loading animation */
        @keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }

        @keyframes slideUp {
            from {
                transform: translateY(50px);
                opacity: 0;
            }
            to {
                transform: translateY(0);
                opacity: 1;
            }
        }
    </style>
</head>
<body>
    <div class="container">
       <h1>Hello <?php echo "world"?></h1>
        <a href="About.php">About</a>
    </div>
</body>
</html>

<?php
require_once "phpv/Parsedown.php";

$pd = new Parsedown();
$page = ""; $css = ""; $head = "";

if (isset($_GET['p'])) {
	$page = 'kernel/' . $_GET['p'] . '.md';
	$css  = 'style.css';
	$head = require_once('html/header-kernel.php');
} else {
	$page = 'index.md';
	$css  = 'home.css';
	$head = require_once('html/header-home.php');
}

$content = $pd->text(file_get_contents($page));

?>

<html>
<head>
	<title> InOS Documentation </title>
	<link rel="preconnect" href="https://fonts.gstatic.com">
	<link href="https://fonts.googleapis.com/css2?family=Open+Sans:wght@300&display=swap" rel="stylesheet">
	<link href="<?= $css ?>" rel="stylesheet">
</head>
<style>
	body {
		font-family: 'Open Sans', sans-serif;
		font-size: 18px;
		line-height: 24px;
	}
</style>
<body>
	<?= $head ?>

	<div class="container">
		<?= $content ?>
	</div>
	
	<br><br>
</body>
</html>

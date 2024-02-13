#!/usr/bin/php
<?php
header("Content-type: text/plain");

echo "<html><body>";

echo "Content-type: text/plain\n\n";
echo "PHP CGI Environment Variables:\n";

foreach ($_SERVER as $key => $value) {
	echo "$key: $value\n";
}
echo "</body></html>";
?>

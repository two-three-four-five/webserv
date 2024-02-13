#!/usr/bin/php
<?php
// 파일 업로드 폴더 설정
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"] ?? '');

// 파일 업로드 체크
if (isset($_POST["submit"]) && !empty($_FILES["fileToUpload"]["name"])) {
    // 파일이 업로드되었는지 확인
    if ($_FILES["fileToUpload"]["error"] == UPLOAD_ERR_OK) {
        // 파일 업로드
        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
            echo "The file ". htmlspecialchars(basename($_FILES["fileToUpload"]["name"])). " has been uploaded.";
        } else {
            echo "Sorry, there was an error uploading your file.";
        }
    } else {
        echo "Error: " . $_FILES["fileToUpload"]["error"];
    }
} else {
    echo "Please select a file to upload.";
}
?>

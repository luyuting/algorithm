<?php
/**
 * File Name:    TestDictSearch.php
 * Author:       luyuting
 * Email:        luyuting415@mail.dlut.edu.cn
 * Created Time: Fri 03 Feb 2017 03:09:06 PM CST
 **/

include_once 'DictSearch.php';

$keys = [];

$fp = fopen('dict.txt', 'r');
if (is_resource($fp)) {
    while (!feof($fp)) {
        $buffer = trim(fgets($fp));
        if ($buffer) {
            $keys[] = $buffer;
        }
    }
    fclose($fp);
}

$word = 'protecting@环境卫生和服务质量，都很大程度地影响人们的点评结果和质量';

$dS = new DictSearch($keys);
$mappedKeys = $dS->search($word);

$keyStr = implode(',', $mappedKeys);
echo $keyStr;

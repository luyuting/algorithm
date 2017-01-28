<?php
include_once 'Apriori.php';

$data = [
    ['a', 'c', 'e'],
    ['b', 'd'],
    ['b', 'c'],
    ['a', 'b', 'c', 'd'],
    ['a', 'b'],
    ['b', 'c'],
    ['a', 'b'],
    ['a', 'b', 'c', 'e'],
    ['a', 'b', 'c'],
    ['a', 'c', 'e']
];

$apriori = Apriori::getInstance();
$relation = $apriori->getRelationRules($data, 0.2, 0.5);

var_dump($relation);

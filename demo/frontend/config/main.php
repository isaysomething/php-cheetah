<?php
$basePath = dirname(__DIR__);
return [
    'id' => 'app-frontend',
    'basePath' => $basePath,
    'components' => [],
    'modules' => [
        'user' => [
            'class' => 'App\\Modules\\User'
        ],
    ],
    'classMap' => [
        'App\\Controllers' => $basePath . DIRECTORY_SEPARATOR . 'controllers',
        'App\\Modules' => $basePath . DIRECTORY_SEPARATOR . 'modules',
    ],
    'params' => []
];
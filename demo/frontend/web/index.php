<?php
$config = array_merge(
    require(__DIR__ . '/../../common/config/main.php'),
    require(__DIR__ . '/../config/main.php')
);
try {
    $application = new \Cheetah\Web\Application($config);
    $application->run();
    var_dump(Cheetah::$logger->messages);
} catch (\Cheetah\Kernel\Exception $e) {
    echo "{$e->getName()}：{$e->getMessage()}<br>";
    echo "{$e->getFile()}：{$e->getLine()}<br>";
}

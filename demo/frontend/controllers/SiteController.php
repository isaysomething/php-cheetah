<?php
/**
 * Created by PhpStorm.
 * User: yrssoft
 * Date: 15-12-22
 * Time: 下午9:17
 */

namespace App\Controllers;


use Cheetah\Web\Controller;

class SiteController extends Controller
{
    public function actionIndex()
    {
        return $this->render('index', [
            'name' => 'yrssoft'
        ]);
    }
}
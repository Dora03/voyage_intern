//
//  ViewController.m
//  ToDoOrNot
//
//  Created by Dora on 2014/10/11.
//  Copyright (c) 2014年 Dora. All rights reserved.
//

#import "ViewController.h"
#import "RDVCalendarView.h"

@interface ViewController ()
// ドロップゾーンのオブジェクト
@property (weak, nonatomic)IBOutlet UIView *dropZone;

// オブジェクトの初期位置を対比しておくための変数を用意する。
@property CGPoint initPosition;
// オブジェクトのドロップ位置を記憶するための変数を用意
@property (assign, nonatomic) CGPoint dropPosition;
@property (strong, nonatomic) NSMutableArray *dropObjArray;
@end

@implementation ViewController

//@synthesize dropPosition;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _dropObjArray = [[NSMutableArray alloc] initWithCapacity:2];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)buttontap:(id)sender
{
    NSLog(@"tapTask");
}
- (IBAction)buttontapPlan:(id)sender
{
    NSLog(@"tapPlan");
}

- (IBAction)dragGesture:(UIPanGestureRecognizer *)sender {
    // stateプロパティはジェスチャーの状態を監視する
    
    if(sender.state == UIGestureRecognizerStateBegan){
        // ドラッグオブジェクトの初期値を覚えておく
        _initPosition = sender.view.center;

        // 事前にUse Auto Layoutは外しておくこと
//        [self.view bringSubviewToFront:sender.view];
//        self.view.alpha = 0.3;
    }

    // 移動量を取得
    CGPoint point = [sender translationInView:self.view];
    
    // 移動量をドラッグしたViewの中心値に加える
    CGPoint movedPoint = CGPointMake(sender.view.center.x + point.x,
                                     sender.view.center.y + point.y);
    sender.view.center = movedPoint;
    
    // ドラッグで移動した距離を初期化する
    [sender setTranslation:CGPointZero inView:self.view];
    
    if(sender.state == UIGestureRecognizerStateEnded
       || sender.state == UIGestureRecognizerStateFailed
       || sender.state == UIGestureRecognizerStateCancelled){
        
        // ドロップゾーンの位置と大きさ
        CGRect dropZoneRect = _dropZone.frame;
        
        // ドラッグするためにタップしている座標を取得
        CGPoint dragPoint = [sender locationInView:self.view];
        
        if(CGRectContainsPoint(dropZoneRect, dragPoint)){
            //カレンダーセレクトしたい
            int DragSelect;
            float DragX, DragY;
            DragSelect = 1; //カレンダーにセレクトされたことを伝える
            DragX = point.x;//セレクトされた座標を代入
            DragY = point.y;
            sender.view.center = _initPosition;
        }
        
        
        if(!CGRectContainsPoint(dropZoneRect, dragPoint)){
            _dropPosition = sender.view.center;
            // ドラッグオブジェクトがドロップゾーン以外にドロップされた場合、初期値にもどす。
            sender.view.center = _initPosition;
        }else{
//            // dropZone内で整列する
//            NSUInteger arrCount = [_dropObjArray count];
//            if(arrCount<2 && ![_dropObjArray containsObject:sender.view]){
//                switch (arrCount) {
//                    case 0:{
//                        sender.view.frame = CGRectMake(
//                                                       dropZoneRect.origin.x+5,
//                                                       dropZoneRect.origin.y+12.5,
//                                                       sender.view.frame.size.width,
//                                                       sender.view.frame.size.height);
//                        break;
//                    }
//                    case 1:{
//                        sender.view.frame = CGRectMake(
//                                                       (dropZoneRect.origin.x+dropZoneRect.size.width)
//                                                       -(sender.view.frame.size.width+5),
//                                                       dropZoneRect.origin.y+12.5,
//                                                       sender.view.frame.size.width,
//                                                       sender.view.frame.size.height);
//                        break;
//                    }
//                }
//                [_dropObjArray addObject:sender.view];
//            }else{
//                // 2つのドラッグオブジェクトがすでにドロップポイントにある場合
//                NSUInteger order = [_dropObjArray indexOfObject:sender.view];
//                
//                // Arrayの順番を入れ替える
//                CGPoint dragObjCenter = sender.view.center;
//                CGRect targetObjRect;
//                switch (order) {
//                    case 0:{
//                        targetObjRect = [[_dropObjArray objectAtIndex:1] frame];
//                        break;
//                    }
//                    case 1:{
//                        targetObjRect = [[_dropObjArray objectAtIndex:0] frame];
//                        break;
//                    }
//                }
//            
//                // ドラッグオブジェクトの中心点がもう一方のオブジェクトに重なったら入れ替え
//                if(CGRectContainsPoint(targetObjRect, dragObjCenter)){
//                    [_dropObjArray exchangeObjectAtIndex:0 withObjectAtIndex:1];
//                }
//                
//                // Arrayの順番に合わせて整頓する
//                for(int i=0;i<arrCount;i++){
//                    UIView *view = [_dropObjArray objectAtIndex:i];
//                    switch (i) {
//                        case 0:{
//                            view.frame = CGRectMake(dropZoneRect.origin.x+5,
//                                                    dropZoneRect.origin.y+12.5,
//                                                    view.frame.size.width,
//                                                    view.frame.size.height);
//                            break;
//                        }
//                        case 1:{
//                            view.frame = CGRectMake(
//                                                    (dropZoneRect.origin.x+dropZoneRect.size.width)
//                                                    -(view.frame.size.width+5),
//                                                    dropZoneRect.origin.y+12.5,
//                                                    view.frame.size.width,
//                                                    view.frame.size.height);
//                            break;
//                        }
//                    }
//                }
//            }
        }
        sender.view.alpha = 1.0;
    }
}

@end

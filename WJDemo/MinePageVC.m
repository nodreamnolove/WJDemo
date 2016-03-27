//
//  MineVC.m
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "MinePageVC.h"
#import "MineCellView.h"
#import "MineCellModel.h"
#import "LoginVC.h"
#import "AppDelegate.h"
#import "MineInfoVC.h"
#import "ModifyVC.h"

@interface MinePageVC ()<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic,strong) UITableView * infoTableview;

@property (nonatomic,strong) NSMutableArray  * cellArr;


@end

@implementation MinePageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"我的";
    [self.view addSubview:self.infoTableview];
    self.automaticallyAdjustsScrollViewInsets = NO;  //
}




//数据
-(NSMutableArray *)cellArr
{
    if (_cellArr == nil) {
        _cellArr = [NSMutableArray arrayWithCapacity:7];
        MineCellModel *modelheader = [MineCellModel new];
        
        modelheader.cellType = MineCellTypeHeader;
        modelheader.title = @"用户名：";
        modelheader.subtitle = @"账户余额：";
        modelheader.leftImage = [UIImage imageNamed:@"headpic"];
        MineCellFrame *headFrame = [MineCellFrame new];
        headFrame.cellModel = modelheader;
        NSArray *section1 = @[headFrame];
        
        MineCellModel *model2 = [MineCellModel new];
        model2.cellType = mineCellTypeBody;
        model2.title = @"我的标签";
        model2.leftImage = [UIImage imageNamed:@"collection"];
        model2.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        MineCellFrame *Frame2 = [MineCellFrame new];
        Frame2.cellModel = model2;
        
        MineCellModel *model3 = [MineCellModel new];
        model3.cellType = mineCellTypeBody;
        model3.leftImage = [UIImage imageNamed:@"photo"];
        model3.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        model3.title = @"密码管理";
        MineCellFrame  *Frame3 = [MineCellFrame new];
        Frame3.cellModel = model3;
        NSArray *section2 = @[Frame2,Frame3];
        
        MineCellModel *model4 = [MineCellModel new];
        model4.cellType = mineCellTypeBody;
        model4.title = @"IC卡充值";
        model4.leftImage = [UIImage imageNamed:@"ambass"];
        model4.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        MineCellFrame  *Frame4 = [MineCellFrame new];
        Frame4.cellModel = model4;
        
        MineCellModel *model5 = [MineCellModel new];
        model5.cellType = mineCellTypeBody;
        model5.title = @"设备连接";
        model5.leftImage = [UIImage imageNamed:@"setup"];
        model5.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        MineCellFrame  *Frame5 = [MineCellFrame new];
        Frame5.cellModel = model5;
        
        MineCellModel *model6 = [MineCellModel new];
        model6.cellType = mineCellTypeBody;
        model6.title = @"我的信息";
        model6.leftImage = [UIImage imageNamed:@"account"];
        model6.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        MineCellFrame  *Frame6 = [MineCellFrame new];
        Frame6.cellModel = model6;
        
        NSArray *section3 = @[Frame4,Frame5,Frame6];        
        [_cellArr addObject:section1];
        [_cellArr addObject:section2];
        [_cellArr addObject:section3];
    }
    return _cellArr;
}


-(UITableView *)infoTableview
{
    if (_infoTableview == nil) {
        _infoTableview = [[UITableView alloc]initWithFrame:CGRectMake(0, 64, ScreenWidth, ScreenHigth-49-64) style:UITableViewStyleGrouped];
        _infoTableview.dataSource = self;
        _infoTableview.delegate = self;
        _infoTableview.backgroundColor = [UIColor colorWithRed:239.0/255.0 green:239.0/255.0 blue:244.0/255.0 alpha:1.0];
//        _infoTableview.tableHeaderView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 1, 1)];
//        _infoTableview.tableFooterView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 1, 1)];
    }
    return _infoTableview;
}


-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return  self.cellArr.count;
}

-(NSInteger )tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.cellArr[section] count];
}

-(MineCellView *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MineCellView *cell = [[MineCellView alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"aa"];
    NSArray * a = self.cellArr[indexPath.section];
    MineCellFrame * b = a[indexPath.row];
    cell.mineCellFrame = b;
    cell.accessoryType = b.cellModel.accessoryType;
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 14;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MineCellFrame *cellFrame = self.cellArr[indexPath.section][indexPath.row];
    return cellFrame.cellFrame.size.height;
}

-(CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    return 6;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    NSString *userName = [AppDelegate instance].userName;
    UIStoryboard *storyBoad = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    if (!userName||userName.length<1) {
       
        LoginVC *loginVC =[storyBoad instantiateViewControllerWithIdentifier:@"LoginVC"];
        loginVC.loginSuccessBlock = ^(NSString *username,NSString *userMoney){
           MineCellFrame * cellFrame = self.cellArr[0][0];
            cellFrame.cellModel.title = [NSString stringWithFormat:@"用户名：%@",username];
            cellFrame.cellModel.subtitle = [NSString stringWithFormat:@"账户余额：%@元",userMoney];
            [self.infoTableview reloadData];
        };
        loginVC.hidesBottomBarWhenPushed = YES;
        [self.navigationController pushViewController:loginVC animated:YES];
        return ;
    }
    if (indexPath.section == 2 && indexPath.row == 2) {
        MineInfoVC *infoVC = [[MineInfoVC alloc]init];
        infoVC.hidesBottomBarWhenPushed = YES;
        [self.navigationController pushViewController:infoVC animated:YES];
    }
    else if (indexPath.section == 1 && indexPath.row == 1){
        
        ModifyVC *modifyVC = [storyBoad instantiateViewControllerWithIdentifier:@"modifyVC"];
        modifyVC.hidesBottomBarWhenPushed = YES;
        [self.navigationController pushViewController:modifyVC animated:YES];
    }
    
    
}

@end

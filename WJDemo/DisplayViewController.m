//
//  DisplayViewController.m
//  WJDemo
//
//  Created by mac on 16/3/21.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "DisplayViewController.h"

@interface DisplayViewController ()<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic, strong) NSMutableArray * dataArr;

@property (nonatomic, strong) UITableView *tableView;
@end

@implementation DisplayViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self.view addSubview:self.tableView];
}
-(NSMutableArray *)dataArr
{
    if (_dataArr==nil) {
        _dataArr = [NSMutableArray array];
        
    }
    return _dataArr;
}
-(UITableView *)tableView
{
    if (_tableView==nil) {
        _tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHigth) style:UITableViewStyleGrouped];
        _tableView.dataSource = self;
        _tableView.delegate = self;
    }
    return _tableView;
}

-(void)setObjdata:(id)objdata
{
    _objdata = objdata;
    if (objdata) {
        
        if ([objdata isKindOfClass:[NSDictionary class]]) {
            NSLog(@"%@",objdata);
            [self.dataArr addObject:objdata];
        }
        else if([objdata isKindOfClass:[NSArray class]])
        {
            NSArray *objArr = (NSArray *)objdata;
            for (int i=0; i<objArr.count; i++) {
                [self.dataArr addObject:objArr[i]];
            }
            
        }
        [self.tableView reloadData];
    }
}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return self.dataArr.count;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSDictionary *dict = self.dataArr[section];
    
    return [dict allKeys].count;
}

-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cellView = [tableView dequeueReusableCellWithIdentifier:@"InfoCell"];
    if (cellView == nil) {
        cellView = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"InfoCell"];
    }
    NSDictionary *dict = self.dataArr[indexPath.section];
    
    NSString* keyVale = [[dict allKeys] objectAtIndex:indexPath.row];
    NSString* valueV = [[dict allValues] objectAtIndex:indexPath.row];
    cellView.textLabel.text = keyVale;
    cellView.detailTextLabel.text = valueV;
    return cellView;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 50;
}
-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 15;
}
-(CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    return 0;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}
@end

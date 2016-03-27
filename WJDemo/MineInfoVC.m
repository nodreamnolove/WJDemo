 

#import "MineInfoVC.h"
#import "HttpGetTool.h"
#import "MBProgressHUD+MJ.h"
#import "AppDelegate.h"

@interface MineInfoVC ()<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic, strong) UITableView *tableView;

@property (nonatomic, strong) NSMutableArray *infoArr;
@property (nonatomic, strong) NSMutableArray *titleArr;

@end

@implementation MineInfoVC

-(UITableView *)tableView{
    
    if (_tableView== nil) {
        _tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, 64, ScreenWidth, ScreenHigth-64) style:UITableViewStylePlain];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        _tableView.separatorColor = [UIColor blackColor];
        _tableView.tableFooterView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 1, 1)];
    }
    return _tableView;
}

-(NSMutableArray *)infoArr{
    if (_infoArr==nil) {
        _infoArr = [NSMutableArray array];
    }
    return _infoArr;
}
-(NSMutableArray *)titleArr{
    if (_titleArr==nil) {
        _titleArr = [NSMutableArray array];
        [_titleArr addObject:@"账号"];
        [_titleArr addObject:@"邮箱"];
        [_titleArr addObject:@"手机号"];
        [_titleArr addObject:@"名字"];
        [_titleArr addObject:@"性别"];
        [_titleArr addObject:@"状态"];
        [_titleArr addObject:@"身份证"];
        [_titleArr addObject:@"余额"];
        [_titleArr addObject:@"注册时间"];
    }
    return _titleArr;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    self.automaticallyAdjustsScrollViewInsets = NO;
    self.title = @"我的信息";
    // Do any additional setup after loading the view.
    NSString *userName = [AppDelegate instance].userName;
    if (!userName||userName.length<1) {
        [MBProgressHUD showError:@"请先登录"];
        return;
    }
    [MBProgressHUD showMessage:@"正在加载数据"];
    [self.view addSubview:self.tableView];
    __weak typeof(self) weakSelf = self;
    [HttpGetTool getUserInfo:userName andSuccess:^(NSDictionary *dict) {
        NSLog(@"%@",[NSThread currentThread]);
        [MBProgressHUD hideHUD];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_account"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_email"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_telephone"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_name"]];
        if ([[dict objectForKey:@"customer_sex"]intValue]==1) {
            [weakSelf.infoArr addObject:@"男"];
        }
        else
            [weakSelf.infoArr addObject:@"女"];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_state"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_idnumber"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_money"]];
        [weakSelf.infoArr addObject:[dict objectForKey:@"customer_registtime"]];
        [weakSelf.tableView reloadData];
    } andFailure:^{
        [MBProgressHUD hideHUD];
        [MBProgressHUD showError:@"获取个人信息失败"];
    }];
}



- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}
-(NSInteger )tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.infoArr.count;
}
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    UITableViewCell *cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"InfoCell"];
    cell.textLabel.text = self.titleArr[indexPath.row];
    cell.detailTextLabel.text = self.infoArr[indexPath.row];
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    return 44;
}

@end

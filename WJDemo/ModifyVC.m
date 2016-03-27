

#import "ModifyVC.h"
#import "HttpGetTool.h"
#import "MBProgressHUD+MJ.h"
#import "AppDelegate.h"

@interface ModifyVC ()

@property (weak, nonatomic) IBOutlet UITextField *userNameField;

@property (weak, nonatomic) IBOutlet UITextField *oldPasswordField;

@property (weak, nonatomic) IBOutlet UITextField *nPasswordField;

@property (weak, nonatomic) IBOutlet UITextField *nPassword2Field;

- (IBAction)submitAction:(id)sender;


@end

@implementation ModifyVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor colorWithRed:239/255.0 green:239/255.0 blue:239/255.0 alpha:1];
    self.userNameField.text = [AppDelegate instance].userName;
    
    
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    
    [self.view endEditing:YES];
}
- (IBAction)submitAction:(id)sender {
    
    if(!self.userNameField.text||self.userNameField.text.length<1){
        [MBProgressHUD showError:@"请输入用户名"];
        return ;
    }
    if(!self.oldPasswordField.text||self.oldPasswordField.text.length<1){
        [MBProgressHUD showError:@"请输入旧密码"];
        return ;
    }
    if(!self.nPasswordField.text||self.nPasswordField.text.length<1){
        [MBProgressHUD showError:@"请输入新密码"];
        return ;
    }
    if(!self.nPassword2Field.text||self.nPassword2Field.text.length<1){
        [MBProgressHUD showError:@"请再次输入新密码"];
        return ;
    }
    if(![self.nPasswordField.text isEqualToString:self.nPassword2Field.text]){
        [MBProgressHUD showError:@"两次输入的新密码不一致"];
        return ;
    }
    
    [HttpGetTool modifyPassword:self.userNameField.text andOldPassword:self.oldPasswordField.text andNewPassword:self.nPasswordField.text andSuccess:^(NSDictionary *dict) {
        if ([[dict objectForKey:@"code"]intValue] == 0) {
            [MBProgressHUD showSuccess:@"修改密码成功"];
        }
    } andFailure:^{
        [MBProgressHUD showError:@"修改密码失败"];
        
    }];
    
    
    
}
@end

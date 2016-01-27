//
//  HomePageVC.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "HomePageVC.h"

@interface HomePageVC ()<UICollectionViewDataSource,UICollectionViewDelegateFlowLayout>

@property (nonatomic, strong) UICollectionView *collectionView;

@end

@implementation HomePageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"首页";
    self.view.backgroundColor = [UIColor clearColor];
}


-(UICollectionView *)collectionView
{
    if (_collectionView == nil) {
        UICollectionViewFlowLayout *fallLayout =[[UICollectionViewFlowLayout alloc]init];
        [fallLayout setScrollDirection:UICollectionViewScrollDirectionVertical];
        _collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(0, 44, ScreenWidth, ScreenHigth-44-20) collectionViewLayout:fallLayout];
        _collectionView.dataSource = self;
        _collectionView.delegate = self;
        [_collectionView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"homePageCell"];
    }
    return _collectionView;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return 9;
}


- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    UICollectionViewCell *collectCell = [collectionView dequeueReusableCellWithReuseIdentifier:@"homePageCell" forIndexPath:indexPath];
    collectCell.backgroundColor = [UIColor blueColor];
    
    return collectCell;
}


@end

//
//  HomePageVC.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "HomePageVC.h"
#import "UIView+HMH.h"

#define PicNum  3
#define ScrollNum 500


@interface HomePageVC ()<UICollectionViewDataSource,UICollectionViewDelegateFlowLayout,UIScrollViewDelegate>

@property (nonatomic,strong) UIScrollView * scrollView;

@property (nonatomic,strong) UIPageControl * pageControl;

@property (nonatomic, strong) UICollectionView *collectionView;

@property (nonatomic,assign) int   currentPage;

@property (nonatomic,assign) int   lastPage;

@property (nonatomic,strong) NSArray * picArr;

@property (nonatomic,strong) UIImageView * imageView0;

@property (nonatomic,strong) UIImageView * imageView1;

@property (nonatomic,strong) UIImageView * imageView2;
@end

@implementation HomePageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"首页";
    self.automaticallyAdjustsScrollViewInsets = NO;
    self.imageView0 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2-1), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView0.image = [UIImage imageNamed:@"obu"];
    self.imageView0.tag = 0;
    
    self.imageView1 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView1.image = [UIImage imageNamed:@"chen"];
    self.imageView1.tag = 1;
    
    self.imageView2 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2+1), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView2.image = [UIImage imageNamed:@"wenhua"];
    self.imageView2.tag = 2;
    
    self.scrollView.contentOffset = CGPointMake((ScrollNum/2)*ScreenWidth, 0);
    [self.scrollView addSubview:self.imageView0];
    [self.scrollView addSubview:self.imageView1];
    [self.scrollView addSubview:self.imageView2];
    
    self.currentPage = (ScrollNum/2);
    self.lastPage = (ScrollNum/2)-1;
    [self.view addSubview:self.scrollView];
    [self.view addSubview:self.pageControl];
}

-(UIScrollView *)scrollView
{
    if (_scrollView == nil) {
        _scrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 64, 320, 80)];
        _scrollView.contentSize = CGSizeMake(ScreenWidth*ScrollNum, ScreenWidth/4);
        _scrollView.backgroundColor = [UIColor clearColor];
        _scrollView.pagingEnabled = YES;
        _scrollView.showsHorizontalScrollIndicator = NO;
        _scrollView.showsVerticalScrollIndicator = NO;
        _scrollView.delegate = self;
        _scrollView.bounces = NO;
        _scrollView.tag = 99;
    }
    return _scrollView;
}

-(UIPageControl *)pageControl
{
    if (_pageControl == nil) {
        _pageControl = [[UIPageControl alloc]initWithFrame:CGRectMake(ScreenWidth/2-50, 64+ScreenWidth/4-20, 100, 10)];
        _pageControl.numberOfPages = PicNum;
        _pageControl.currentPage = 0;
        [_pageControl addTarget:self action:@selector(pageChange:) forControlEvents:UIControlEventValueChanged];
    }
    return _pageControl;
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

#pragma mark scrollect 代理
-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    int page = floor((scrollView.contentOffset.x - ScreenWidth/2)/ScreenWidth)+1;
    self.pageControl.currentPage = page%PicNum;
    UIImageView *lastView;
    CGFloat nextViewx;
    NSLog(@"page= %d,currentPage=%d,lastpage=%d",page,self.currentPage,self.lastPage);
    if ((page)!=self.currentPage) {
        
        if(page > self.currentPage)
        {
            if (self.currentPage > self.lastPage)  //1.正向
            {
                lastView = [scrollView viewWithTag:self.lastPage%PicNum];
                nextViewx = (page+1)%ScrollNum * ScreenWidth;
            }
            else                                //4.先反向后正向
            {
                lastView = [scrollView viewWithTag:(self.currentPage-1+PicNum)%PicNum];
                nextViewx = (page + 1)%ScrollNum *ScreenWidth;
               
            }
            
        }
       else if (page < self.currentPage)
       {
            if (self.lastPage > self.currentPage) //2.反向
            {
                lastView = [scrollView viewWithTag:self.lastPage%PicNum];
                nextViewx = (page-1+ScrollNum)%ScrollNum * ScreenWidth;
                
                
            }
            else                                //3.先正向后反向
            {
                lastView = [scrollView viewWithTag:(self.currentPage+1+PicNum)%PicNum];
                nextViewx = (page-1+ScrollNum)%ScrollNum *ScreenWidth;
                
            }
        }
        [lastView setViewX:nextViewx];
        self.lastPage = self.currentPage;
        self.currentPage = page;
        if (self.currentPage == 0|| self.currentPage == ScrollNum-1) {
            self.currentPage = ScrollNum/2;
            self.lastPage = ScrollNum/2-1;
            self.scrollView.contentOffset = CGPointMake(ScreenWidth*(ScrollNum/2), ScreenWidth/4);
            [self.imageView0 setViewX:ScreenWidth*(ScrollNum/2-1) ];
            [self.imageView1 setViewX:ScreenWidth*(ScrollNum/2)];
            [self.imageView2 setViewX:ScreenWidth*(ScrollNum/2+1)];
            self.pageControl.currentPage = self.currentPage%PicNum;
        }
    }
}

#pragma mark collection代理
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return 9;
}


- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    UICollectionViewCell *collectCell = [collectionView dequeueReusableCellWithReuseIdentifier:@"homePageCell" forIndexPath:indexPath];
    collectCell.backgroundColor = [UIColor blueColor];
    return collectCell;
}

#pragma mark 点击pagecontrol

-(void)pageChange:(id)sender
{
    
}

@end

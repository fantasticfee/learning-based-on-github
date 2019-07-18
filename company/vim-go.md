#vim go 配置

## 一、使用vundle管理插件
```
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
```

## 二、vimrc配置
```
colorscheme evening

set fileformats=unix
set shiftwidth=1
set nu
set hlsearch

set nocompatible
set relativenumber
nnoremap k gk
nnoremap gk k
nnoremap j gj
nnoremap gj j

filetype off
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'fatih/vim-go'

call vundle#end()
filetype plugin indent on
```
## 三、插件下载
执行install.sh

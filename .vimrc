" required settings for Vundle
set  nocompatible 
filetype off
set mouse=a
set encoding=UTF-8

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

" let Vundle manage Vundle, required
" convenient plugin management 
Plugin 'VundleVim/Vundle.vim'
" automatic code completion
Plugin 'Valloric/YouCompleteMe'
" run GDB within vim
Plugin 'vim-scripts/Conque-GDB'
" quick generation of doxygen comments
Plugin 'vim-scripts/DoxygenToolkit.vim'
" quickly find files with ctrl+p
Plugin 'ctrlpvim/ctrlp.vim'
" vim cheat sheet, opened with \?
Plugin 'lifepillar/vim-cheat40'
" quick file tree
Plugin 'scrooloose/nerdtree'
" pretty status bar
Plugin 'itchyny/lightline.vim'
" Markdown editing
Plugin 'godlygeek/tabular'
Plugin 'plasticboy/vim-markdown'
" Preview markdown files with Ctrl-P
Plugin 'JamshedVesuna/vim-markdown-preview'
call vundle#end()

" Use github-style markdown, previewed in firefox.
let vim_markdown_preview_github=1
let vim_markdown_preview_browser='Mozilla Firefox'
let vim_markdown_preview_use_xdg_open=1

" Open NERDtree when opening vim without picking a file
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists("s:std_in") | exe 'NERDTree' argv()[0] | wincmd p | ene | endif
 
" Indentation
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab
set autoindent
set smartindent
set cindent
set formatoptions+=r

" Code readability
syntax on

" read custom .vimrc from any folder
set exrc
" restrict certain commands
set secure

" exit insert mode with jj
inoremap jj <ESC>

" Some convenient shortcuts recommended by https://aonemd.github.io/blog/handy-keymaps-in-vim
let mapleader="\<Space>"
" Save current buffer
nnoremap <leader>w :w<cr>
" Replace the word under cursor
nnoremap <leader>* :%s/\<<c-r><c-w>\>//g<left><left>
" Move lines around
nnoremap <leader>k :m-2<cr>==
nnoremap <leader>j :m+<cr>==
xnoremap <leader>k :m-2<cr>gv=gv
xnoremap <leader>j :m'>+<cr>gv=gv

" set comments+=s:/**,mb:\s*,ex:\s*/
" set comments+=s:/*,mb:\s*,ex:\s*/
"F1: open cheat sheets
map <F1> <leader>?<cr>
noremap <F2> :NERDTreeToggle<cr> 
map <F11> :set invnumber<cr>

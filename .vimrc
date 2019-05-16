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
set number
set colorcolumn=81
" Make YcmCompleter use Python2
let g:ycm_server_python_interpreter="python2"
" read custom .vimrc from any folder
set exrc
" restrict certain commands
set secure
" exit insert mode with jj
inoremap jj <ESC>
" prefix function definitions with a class/namespace name
command -nargs=1 Nametag %s/\(\*\/[\n\r][a-zA-Z_:\-]\+ \)\([a-zA-Z_-]\+(\)/\1<args>::\2/gc
" build tools and project settings
augroup cpp_files
    au!
    autocmd BufNewFile,BufRead *.h,*.cpp set syntax=cpp.doxygen
augroup END
set makeprg=make
" set comments+=s:/**,mb:\s*,ex:\s*/
" set comments+=s:/*,mb:\s*,ex:\s*/
"F1: open cheat sheets
map <F1> <leader>?<cr>
noremap <F2> :NERDTreeToggle<cr>
noremap <F3> :YcmCompleter GoTo<cr>
noremap <F4> :make<cr>
noremap <F7> :Dox<cr>
noremap <F8> :tabnew<cr>
map <F11> :set invnumber<cr>

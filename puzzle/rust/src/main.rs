// puzzle/rust//main.rs

use indicatif::{ProgressBar, ProgressStyle};
use rayon::prelude::*;
use std::collections::{HashMap, VecDeque};
use std::fs::File;
use std::io::{BufWriter, Write};
use std::sync::Mutex;
use std::time::Instant;

const GOAL: &str = "123456780";
const DIRS: [(isize, isize); 4] = [(-1, 0), (0, 1), (1, 0), (0, -1)];
const DIR_CHARS: [char; 4] = ['0', '1', '2', '3'];

fn get_moves(pos: usize) -> Vec<(usize, usize)>
{
    let x = (pos % 3) as isize;
    let y = (pos / 3) as isize;
    let mut result = Vec::new();
    for (i, &(dy, dx)) in DIRS.iter().enumerate()
    {
        let nx = x + dx;
        let ny = y + dy;
        if nx >= 0 && nx < 3 && ny >= 0 && ny < 3
        {
            let new_pos = (ny * 3 + nx) as usize;
            result.push((i, new_pos));
        }
    }
    result
}

fn bfs(start: &str) -> Option<String>
{
    let mut visited = HashMap::new();
    let mut queue = VecDeque::new();

    visited.insert(start.to_string(), "".to_string());
    queue.push_back(start.to_string());

    while let Some(state) = queue.pop_front()
    {
        if state == GOAL
        {
            return visited.get(&state).cloned();
        }
        let zero_pos = state.find('0').unwrap();
        for (dir, new_pos) in get_moves(zero_pos)
        {
            let mut new_state = state.chars().collect::<Vec<_>>();
            new_state.swap(zero_pos, new_pos);
            let new_state_str: String = new_state.iter().collect();
            if !visited.contains_key(&new_state_str)
            {
                let mut path = visited.get(&state).unwrap().clone();
                path.push(DIR_CHARS[dir]);
                visited.insert(new_state_str.clone(), path);
                queue.push_back(new_state_str);
            }
        }
    }
    None
}

fn main()
{
    let start_time = Instant::now();
    let file = File::create("solutions.txt").expect("无法创建输出文件");
    let writer = Mutex::new(BufWriter::new(file));

    let mut all_states = Vec::new();
    let mut perm = "123456780".chars().collect::<Vec<_>>();
    permutohedron::heap_recursive(&mut perm, |p| {
        let s: String = p.iter().collect();
        all_states.push(s);
    });

    let progress = ProgressBar::new(all_states.len() as u64);
    progress.set_style(
        ProgressStyle::default_bar()
            .template("[{elapsed_precise}] [{wide_bar}] {pos}/{len} ({eta})")
            .unwrap()
            .progress_chars("=>-"),
    );

    all_states.par_iter().for_each(|state| {
        if let Some(path) = bfs(state)
        {
            let mut writer = writer.lock().unwrap();
            writeln!(writer, "{}:{}", state, path).unwrap();
        }
        else
        {
            let mut writer = writer.lock().unwrap();
            writeln!(writer, "{}:{}", state, "x").unwrap();
        }
        progress.inc(1);
    });

    progress.finish();
    println!("所有路径保存完成，总耗时：{:?}", start_time.elapsed());
}

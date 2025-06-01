function generateEllerMaze(rows, cols) {
  const vWalls = Array.from({length: rows}, () => Array(cols - 1).fill(0));
  const hWalls = Array.from({length: rows - 1}, () => Array(cols).fill(0));
  const set = Array(cols).fill(0);
  let uniques = 1;

  for (let j = 0; j < cols; j++) {
    set[j] = uniques++;
  }
  for (let i = 0; i < rows - 1; i++) {
    makeVerticalWalls(vWalls, set, i, cols);
    for (let j = 0; j < cols; j++) {
      hWalls[i][j] = Math.random() > 0.5 ? 1 : 0;
    }
    checkHorizontalPass(hWalls, set, i, cols, uniques);
    for (let j = 0; j < cols; j++) {
      if (hWalls[i][j]) {
        set[j] = uniques++;
      }
    }
  }

  makeVerticalWalls(vWalls, set, rows - 1, cols);
  for (let j = 0; j < cols - 1; j++) {
    if (set[j] !== set[j + 1]) {
      vWalls[rows - 1][j] = 0;
      const changingSet = set[j + 1];
      for (let k = 0; k < cols; k++) {
        if (set[k] === changingSet) set[k] = set[j];
      }
    }
    hWalls[rows - 1] = hWalls[rows - 1] || [];
    hWalls[rows - 1][j] = 1;
  }
  hWalls[rows - 1][cols - 1] = 1;
  return {vWalls, hWalls};
}

function makeVerticalWalls(vWalls, set, i, cols) {
  for (let j = 0; j < cols - 1; j++) {
    if (set[j] === set[j + 1]) {
      vWalls[i][j] = 1;
    } else {
      vWalls[i][j] = Math.random() > 0.5 ? 1 : 0;
    }
    if (!vWalls[i][j]) {
      const changingSet = set[j + 1];
      for (let k = 0; k < cols; k++) {
        if (set[k] === changingSet) set[k] = set[j];
      }
    }
  }
}

function checkHorizontalPass(hWalls, set, i, cols, uniques) {
  for (let s = 1; s < uniques; s++) {
    const cells = [];
    for (let j = 0; j < cols; j++) {
      if (set[j] === s) cells.push(j);
    }
    if (cells.length) {
      let passFound = false;
      for (const cell of cells) {
        if (!hWalls[i][cell]) passFound = true;
      }
      if (!passFound) {
        hWalls[i][cells[Math.floor(Math.random() * cells.length)]] = 0;
      }
    }
  }
}

function drawMaze(vWalls, hWalls, rows, cols) {
  const canvas = document.getElementById('mazeCanvas');
  const ctx = canvas.getContext('2d');
  const cellWidth = canvas.width / cols;
  const cellHeight = canvas.height / rows;

  ctx.clearRect(0, 0, canvas.width, canvas.height);

  ctx.strokeStyle = 'black';
  ctx.lineWidth = 1;
  ctx.strokeRect(0.5, 0.5, canvas.width - 1, canvas.height - 1);
  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols - 1; j++) {
      if (vWalls[i][j]) {
        ctx.beginPath();
        ctx.moveTo((j + 1) * cellWidth + 0.5, i * cellHeight + 0.5);
        ctx.lineTo((j + 1) * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.stroke();
      }
    }
  }
  for (let i = 0; i < rows - 1; i++) {
    for (let j = 0; j < cols; j++) {
      if (hWalls[i][j]) {
        ctx.beginPath();
        ctx.moveTo(j * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.lineTo((j + 1) * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.stroke();
      }
    }
  }
}

class Point {
  constructor(row, col) {
    this.row = row;
    this.col = col;
  }
}

class Queue {
  constructor() {
    this.data = [];
    this.head = 0;
    this.tail = 0;
  }
  empty() {
    return this.head === this.tail;
  }
  enqueue(p) {
    this.data[this.tail++] = p;
  }
  dequeue() {
    return this.data[this.head++];
  }
}

function canGo(maze, r1, c1, r2, c2, vWalls, hWalls) {
  if (r2 < 0 || r2 >= maze.rows || c2 < 0 || c2 >= maze.cols) return false;
  if (r1 === r2 && c1 === c2 + 1) return !vWalls[r2][c2];  // left
  if (r1 === r2 && c1 + 1 === c2) return !vWalls[r1][c1];  // right
  if (r1 + 1 === r2 && c1 === c2) return !hWalls[r1][c1];  // down
  if (r1 === r2 + 1 && c1 === c2) return !hWalls[r2][c2];  // up
  return false;
}

let maze = {rows: 20, cols: 30};
let vWalls = [];
let hWalls = [];
let start = new Point(0, 0);
let end = new Point(19, 29);
let path = [];
let currentPathStep = 0;
let isAnimatingPath = false;
let pathAnimationId = null;


function bfs(maze, start, end, vWalls, hWalls) {
  const rows = maze.rows;
  const cols = maze.cols;
  const prev = Array.from({length: rows}, () => Array(cols).fill(null));
  const q = new Queue();
  const dr = [-1, 1, 0, 0];
  const dc = [0, 0, -1, 1];

  q.enqueue(start);
  prev[start.row][start.col] = start;

  let found = false;
  while (!q.empty() && !found) {
    const cur = q.dequeue();
    if (cur.row === end.row && cur.col === end.col) {
      found = true;
    } else {
      for (let d = 0; d < 4; d++) {
        const nr = cur.row + dr[d];
        const nc = cur.col + dc[d];
        if (canGo(maze, cur.row, cur.col, nr, nc, vWalls, hWalls) &&
            !prev[nr][nc]) {
          prev[nr][nc] = cur;
          q.enqueue(new Point(nr, nc));
        }
      }
    }
  }

  if (!found) return [];

  const path = [];
  let p = end;
  while (p.row !== start.row || p.col !== start.col) {
    path.push(p);
    p = prev[p.row][p.col];
  }
  path.push(start);
  return path.reverse();
}

function drawPoint(point, color) {
  const canvas = document.getElementById('mazeCanvas');
  const ctx = canvas.getContext('2d');
  const cellWidth = canvas.width / maze.cols;
  const cellHeight = canvas.height / maze.rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;
  const x = (point.col + 0.5) * cellWidth - size / 2;
  const y = (point.row + 0.5) * cellHeight - size / 2;
  ctx.fillStyle = color;
  ctx.fillRect(x, y, size, size);
}

function generateMaze() {
  const rows = parseInt(document.getElementById('rows').value);
  const cols = parseInt(document.getElementById('cols').value);
  maze = {rows, cols};
  const result = generateEllerMaze(rows, cols);
  vWalls = result.vWalls;
  hWalls = result.hWalls;
  drawMaze(vWalls, hWalls, rows, cols);
  start = null;
  end = null;
  path = [];
  currentStep = 0;
}

function selectStart() {
  const row = parseInt(document.getElementById('startRow').value);
  const col = parseInt(document.getElementById('startCol').value);
  if (isNaN(row) || isNaN(col)) {
    alert('Input valid start point!');
    return;
  }
  if (row < 1 || row > maze.rows || col < 1 || col > maze.cols) {
    alert('Start point is out of maze!');
    return;
  }
  start = new Point(row - 1, col - 1);
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  if (start) drawPoint(start, 'green');
  if (end) drawPoint(end, 'red');
  path = [];
  currentStep = 0;
}

function selectEnd() {
  const row = parseInt(document.getElementById('endRow').value);
  const col = parseInt(document.getElementById('endCol').value);
  if (isNaN(row) || isNaN(col)) {
    alert('Input valid end point!');
    return;
  }
  if (row < 1 || row > maze.rows || col < 1 || col > maze.cols) {
    alert('End point is out of maze!');
    return;
  }
  end = new Point(row - 1, col - 1);
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  if (start) drawPoint(start, 'green');
  if (end) drawPoint(end, 'red');
  path = [];
  currentStep = 0;
}

function solveMaze() {
  if (!start || !end) {
    alert('Set start and end points!');
    return;
  }
  path = [];
  currentStep = 0;
  path = bfs(maze, start, end, vWalls, hWalls);

  if (path.length === 0) {
    alert('Путь не найден!');
    return;
  }
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  drawPath(path, maze);
  drawPoint(start, 'green');
  drawPoint(end, 'red');
}

function drawPathStep() {
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  drawPoint(start, 'green');
  drawPoint(end, 'red');

  for (let i = 0; i <= currentPathStep; i++) {
    drawPoint(path[i], 'rgb(255, 0, 0)');
  }
  currentPathStep++;
  if (currentPathStep >= path.length) {
    isAnimatingPath = false;
    if (pathAnimationId) {
      clearTimeout(pathAnimationId);
      pathAnimationId = null;
    }
    stopPathAnimation();
    return;
  }
  const ms = parseInt(document.getElementById('delayMs').value) || 200;
  pathAnimationId = setTimeout(drawPathStep, ms);
}

function animatePath() {
  if (path.length === 0) return;
  isAnimatingPath = true;
  drawPathStep();
}

function stopPathAnimation() {
  isAnimatingPath = false;
  if (pathAnimationId) {
    clearTimeout(pathAnimationId);
    pathAnimationId = null;
  }
  document.getElementById('solveMaze').textContent = 'Solve maze';
}


document.addEventListener('DOMContentLoaded', function() {
  resetInputsToDefaults();
  const rows = parseInt(document.getElementById('rows').value);
  const cols = parseInt(document.getElementById('cols').value);
  maze = {rows, cols};
  start = new Point(0, 0);
  end = new Point(rows - 1, cols - 1);
  const result = generateEllerMaze(rows, cols);
  vWalls = result.vWalls;
  hWalls = result.hWalls;
  drawMaze(vWalls, hWalls, rows, cols);
  drawPoint(start, 'green');
  drawPoint(end, 'red');

  document.getElementById('generateMaze').onclick = generateMaze;
  document.getElementById('selectStart').onclick = selectStart;
  document.getElementById('selectEnd').onclick = selectEnd;
});

function resetInputsToDefaults() {
  document.getElementById('rows').value = 25;
  document.getElementById('cols').value = 50;
  document.getElementById('startRow').value = 1;
  document.getElementById('startCol').value = 1;
  document.getElementById('endRow').value = 25;
  document.getElementById('endCol').value = 50;
}

document.getElementById('solveMaze').onclick = function() {
  if (isAnimatingPath) {
    stopPathAnimation();
    this.textContent = 'Solve maze';
  } else {
    if (path.length === 0) {
      path = bfs(maze, start, end, vWalls, hWalls);
      if (path.length === 0) {
        alert('Can not find passage');
        return;
      }
      currentPathStep = 1;
    }
    this.textContent = 'Stop';
    animatePath();
  }
};

document.getElementById('nextStep').onclick = function() {
  if (isAnimatingPath) {
    stopPathAnimation();
  }
  if (path.length === 0) {
    path = bfs(maze, start, end, vWalls, hWalls);
    currentPathStep = 1;
  }
  if (currentPathStep >= path.length) {
    alert('The passage is completed!');
    path = [];
    return;
  }
  drawPoint(path[currentPathStep], 'rgb(255, 0, 0)');
  currentPathStep++;
};

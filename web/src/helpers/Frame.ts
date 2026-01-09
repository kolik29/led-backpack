import Frame from "../types/Frame";

function hexToRgb(hex: string) {
  if (!hex || hex[0] !== "#" || hex.length !== 7) return { r: 0, g: 0, b: 0 };
  
  const r = parseInt(hex.slice(1, 3), 16);
  const g = parseInt(hex.slice(3, 5), 16);
  const b = parseInt(hex.slice(5, 7), 16);
  
  if (Number.isNaN(r) || Number.isNaN(g) || Number.isNaN(b)) return { r: 0, g: 0, b: 0 };
  
  return { r, g, b };
}

export function drawFrame(
  canvas: HTMLCanvasElement,
  frame: Frame,
  drawGrid = true
) {
  const { w, h, pixels } = frame;

  const ctx = canvas.getContext("2d");
  if (!ctx) return;

  const W = canvas.clientWidth || canvas.width;
  const H = canvas.clientHeight || canvas.height;

  if (W === 0 || H === 0) return;

  if (canvas.width !== W) canvas.width = W;
  if (canvas.height !== H) canvas.height = H;

  ctx.imageSmoothingEnabled = false;
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  const cellX = canvas.width / w;
  const cellY = canvas.height / h;

  for (let y = 0; y < h; y++) {
    for (let x = 0; x < w; x++) {
      const i = y * w + x;
      const c = pixels[i] ?? "#000000";
      const { r, g, b } = hexToRgb(c);

      ctx.fillStyle = `rgb(${r},${g},${b})`;
      ctx.fillRect(x * cellX, y * cellY, cellX, cellY);
    }
  }

  if (drawGrid && Math.min(cellX, cellY) >= 4) {
    ctx.save();
    ctx.strokeStyle = "rgba(0,0,0,0.25)";
    ctx.lineWidth = 1;

    ctx.beginPath();

    for (let x = 0; x <= w; x++) {
      const px = x * cellX + 0.5;
      ctx.moveTo(px, 0);
      ctx.lineTo(px, canvas.height);
    }

    for (let y = 0; y <= h; y++) {
      const py = y * cellY + 0.5;
      ctx.moveTo(0, py);
      ctx.lineTo(canvas.width, py);
    }

    ctx.stroke();
    ctx.restore();
  }
}


export function emptyFrame(w: number, h: number, color = "#000000"): Frame {
  return { w, h, pixels: Array.from({ length: w * h }, () => color) };
}
import { getText, postJson } from "../helpers/Http";
import Frame from "../types/Frame";

export type Mode = "frame" | "text" | "effect";

export async function ping() {
  return getText("/api/ping");
}

export async function info() {
  return postJson("/api/info", {});
}

export async function reboot() {
  return postJson("/api/reboot", {});
}

export async function setBrightness(brightness: number) {
  return postJson("/api/brightness", { brightness });
}

export async function sendFrame(frame: Frame) {
  return postJson("/api/frame", frame);
}

export async function clear() {
  return postJson("/api/clear", {});
}

export async function setMode(mode: Mode, effect?: string) {
  const body: any = { mode };
  if (mode === "effect" && effect) body.effect = effect;
  return postJson("/api/mode", body);
}

export async function setText(payload: {
  text: string;
  color?: string;
  speed?: number;
  scroll?: boolean;
  spacing?: number;
  y?: number;
}) {
  return postJson("/api/text", payload);
}

export async function setEffect(payload: {
  effect: "flashlight" | "colorshift" | "fire";
  params?: Record<string, any>;
}) {
  return postJson("/api/effect", payload);
}
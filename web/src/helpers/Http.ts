import { ApiError } from '../types/ApiError';

async function parseJsonSafe(res: Response) {
  const ct = res.headers.get('content-type') || '';
  if (ct.includes('application/json')) return res.json();
  const text = await res.text();
  return text;
}

export async function postJson<TResp = any>(url: string, body: unknown): Promise<TResp> {
  const res = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });

  const data = await parseJsonSafe(res);

  if (!res.ok) {
    const msg =
      typeof data === 'string'
        ? data
        : (data as any)?.error || (data as any)?.message || `HTTP ${res.status}`;

    const err: ApiError = { status: res.status, message: msg, body: data };
    throw err;
  }

  return data as TResp;
}

export async function getText(url: string): Promise<string> {
  const res = await fetch(url);
  const text = await res.text();
  if (!res.ok) throw { status: res.status, message: text } as ApiError;
  return text;
}
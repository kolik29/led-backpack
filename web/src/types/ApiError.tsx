export type ApiError = {
  status: number;
  message: string;
  body?: unknown;
};